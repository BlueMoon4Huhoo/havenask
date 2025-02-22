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
#include "build_service/worker/WorkerHeartbeat.h"

#include <iosfwd>

using namespace std;

namespace build_service { namespace worker {
BS_LOG_SETUP(worker, WorkerHeartbeat);

WorkerHeartbeat::WorkerHeartbeat(proto::WorkerNodeBase* workerNode) : _workerNode(workerNode) {}

WorkerHeartbeat::~WorkerHeartbeat() {}

}} // namespace build_service::worker
