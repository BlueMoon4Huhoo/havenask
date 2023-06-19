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
#ifndef __INDEXLIB_DICTIONARY_SCHEMA_H
#define __INDEXLIB_DICTIONARY_SCHEMA_H

#include <memory>

#include "indexlib/common_define.h"
#include "indexlib/index/inverted_index/config/DictionaryConfig.h"
#include "indexlib/indexlib.h"

DECLARE_REFERENCE_CLASS(config, DictionarySchemaImpl);

namespace indexlib { namespace config {

class DictionarySchema : public autil::legacy::Jsonizable
{
public:
    typedef std::map<std::string, std::shared_ptr<DictionaryConfig>>::const_iterator Iterator;

public:
    DictionarySchema();
    ~DictionarySchema();

public:
    void AddDictionaryConfig(const std::shared_ptr<DictionaryConfig>& dictConfig);
    std::shared_ptr<DictionaryConfig> GetDictionaryConfig(const std::string& dictName) const;
    void Jsonize(autil::legacy::Jsonizable::JsonWrapper& json) override;
    void AssertEqual(const DictionarySchema& other) const;
    void AssertCompatible(const DictionarySchema& other) const;

    Iterator Begin() const;
    Iterator End() const;

private:
    DictionarySchemaImplPtr mImpl;

private:
    IE_LOG_DECLARE();
};

DEFINE_SHARED_PTR(DictionarySchema);
}} // namespace indexlib::config

#endif //__INDEXLIB_DICTIONARY_SCHEMA_H