/*
 * Copyright 2014-present Alibaba Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "indexlib/config/impl/source_schema_impl.h"

#include "indexlib/config/config_define.h"
#include "indexlib/config/configurator_define.h"
#include "indexlib/util/Exception.h"

using namespace std;
using namespace autil;
using namespace autil::legacy;
using namespace autil::legacy::json;

namespace indexlib { namespace config {
IE_LOG_SETUP(config, SourceSchemaImpl);

SourceSchemaImpl::SourceSchemaImpl() : mIsAllFieldsDisabled(false) {}

SourceSchemaImpl::~SourceSchemaImpl() {}

void SourceSchemaImpl::Jsonize(autil::legacy::Jsonizable::JsonWrapper& json)
{
    json.Jsonize(SOURCE_CONFIG_MODULES, mModules, mModules);
    json.Jsonize(SOURCE_GROUP_CONFIGS, mGroupConfigs, mGroupConfigs);
    if (json.GetMode() == FROM_JSON) {
        for (size_t gid = 0; gid < mGroupConfigs.size(); gid++) {
            mGroupConfigs[gid]->SetGroupId(gid);
        }
    }
}

void SourceSchemaImpl::AssertEqual(const SourceSchemaImpl& other) const
{
    for (size_t i = 0; i < mGroupConfigs.size(); i++) {
        auto status = mGroupConfigs[i]->CheckEqual(*(other.mGroupConfigs[i].get()));
        if (!status.IsOK()) {
            AUTIL_LEGACY_THROW(util::AssertEqualException, "group config not equal");
        }
    }
    IE_CONFIG_ASSERT_EQUAL(mModules, other.mModules, "Modules not equal");
    IE_CONFIG_ASSERT_EQUAL(mIsAllFieldsDisabled, other.mIsAllFieldsDisabled, "disable fields not equal");
}

void SourceSchemaImpl::Check() const
{
    if (mGroupConfigs.empty()) {
        INDEXLIB_FATAL_ERROR(Schema, "SourceSchema should have at least one group config.");
    }

    for (size_t i = 0; i < mGroupConfigs.size(); ++i) {
        SourceGroupConfigPtr groupConfig = mGroupConfigs[i];
        const string& moduleName = groupConfig->GetModule();
        if (moduleName.empty()) {
            continue;
        }
        if (!IsModuleExist(moduleName)) {
            // this group use non-existed modul
            INDEXLIB_FATAL_ERROR(Schema, "SourceGroupConfig[%lu] use invaild module [%s]", i, moduleName.c_str());
        }
    }
}

bool SourceSchemaImpl::IsModuleExist(const string& moduleName) const
{
    for (auto& module : mModules) {
        if (module.moduleName == moduleName) {
            return true;
        }
    }
    return false;
}

bool SourceSchemaImpl::GetModule(const string& moduleName, ModuleInfo& ret) const
{
    for (auto& module : mModules) {
        if (module.moduleName == moduleName) {
            ret = module;
            return true;
        }
    }
    return false;
}

void SourceSchemaImpl::AddGroupConfig(const SourceGroupConfigPtr& groupConfig) { mGroupConfigs.push_back(groupConfig); }

const SourceGroupConfigPtr& SourceSchemaImpl::GetGroupConfig(index::groupid_t groupId) const
{
    if (groupId < 0 || groupId >= (index::groupid_t)mGroupConfigs.size()) {
        static SourceGroupConfigPtr emptyConfig;
        return emptyConfig;
    }
    return mGroupConfigs[groupId];
}

void SourceSchemaImpl::DisableAllFields()
{
    mIsAllFieldsDisabled = true;
    for (auto group : mGroupConfigs) {
        group->SetDisabled(true);
    }
}

bool SourceSchemaImpl::IsAllFieldsDisabled() const { return mIsAllFieldsDisabled; }

size_t SourceSchemaImpl::GetSourceGroupCount() const { return mGroupConfigs.size(); }

void SourceSchemaImpl::DisableFieldGroup(index::groupid_t groupId)
{
    if (groupId < 0 || groupId >= mGroupConfigs.size()) {
        IE_LOG(WARN, "group id [%d] for source index not exist.", groupId);
        return;
    }

    mGroupConfigs[groupId]->SetDisabled(true);
    for (auto groupConfig : mGroupConfigs) {
        if (!groupConfig->IsDisabled()) {
            mIsAllFieldsDisabled = false;
            return;
        }
    }
    mIsAllFieldsDisabled = true;
}

void SourceSchemaImpl::GetDisableGroupIds(std::vector<index::groupid_t>& ids) const
{
    assert(ids.empty());
    for (auto groupConfig : mGroupConfigs) {
        if (groupConfig->IsDisabled()) {
            ids.push_back(groupConfig->GetGroupId());
        }
    }
}
}} // namespace indexlib::config