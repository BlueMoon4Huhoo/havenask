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
#ifndef __INDEXLIB_DEFAULT_SPLIT_STRATEGY_H
#define __INDEXLIB_DEFAULT_SPLIT_STRATEGY_H

#include <memory>

#include "indexlib/common_define.h"
#include "indexlib/indexlib.h"
#include "indexlib/merger/split_strategy/split_segment_strategy.h"

namespace indexlib { namespace merger {

class DefaultSplitStrategy : public SplitSegmentStrategy
{
public:
    DefaultSplitStrategy(index::SegmentDirectoryBasePtr segDir, config::IndexPartitionSchemaPtr schema,
                         index::OfflineAttributeSegmentReaderContainerPtr attrReaders,
                         const index_base::SegmentMergeInfos& segMergeInfos, const MergePlan& plan,
                         std::map<segmentid_t, index::SegmentMetricsUpdaterPtr> hintDocInfos,
                         const util::MetricProviderPtr& metrics);

    ~DefaultSplitStrategy();

public:
    bool Init(const util::KeyValueMap& parameters) override;
    segmentindex_t DoProcess(segmentid_t oldSegId, docid_t oldLocalId, int64_t& hintValue) override;

public:
    static const std::string STRATEGY_NAME;

private:
    size_t mTotalValidDocCount;
    size_t mSplitDocCount;
    size_t mCurrentDocCount;
    size_t mCurrentSegIndex;

private:
    IE_LOG_DECLARE();
};

DEFINE_SHARED_PTR(DefaultSplitStrategy);
}} // namespace indexlib::merger

#endif //__INDEXLIB_DEFAULT_SPLIT_STRATEGY_H