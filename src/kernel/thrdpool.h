/*
  Copyright (c) 2019 Sogou, Inc.

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

      http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

  Author: Xie Han (xiehan@sogou-inc.com)
*/


/* 特点1：创建完线程池后，无需记录任何线程id或对象，线程池可以通过一个等一个的方式优雅地去结束所有线程；
特点2：线程任务可以由另一个线程任务调起；甚至线程池正在被销毁时也可以提交下一个任务；（这很重要，因为线程本身很可能是不知道线程池的状态的；
特点3：同理，线程任务也可以销毁这个线程池；（非常完整～ */
// https://segmentfault.com/a/1190000041747964


#ifndef _THRDPOOL_H_
#define _THRDPOOL_H_

#include <stddef.h>

typedef struct __thrdpool thrdpool_t;

struct thrdpool_task
{
	void (*routine)(void *);
	void *context;
};

#ifdef __cplusplus
extern "C"
{
#endif

/*
 * Thread pool originates from project Sogou C++ Workflow
 * https://github.com/sogou/workflow
 *
 * A thread task can be scheduled by another task, which is very important,
 * even if the pool is being destroyed. Because thread task is hard to know
 * what's happening to the pool.
 * The thread pool can also be destroyed by a thread task. This may sound
 * strange, but it's very logical. Destroying thread pool in thread task
 * does not end the task thread. It'll run till the end of task.
 */

thrdpool_t *thrdpool_create(size_t nthreads, size_t stacksize);
int thrdpool_schedule(const struct thrdpool_task *task, thrdpool_t *pool);
int thrdpool_in_pool(thrdpool_t *pool);
int thrdpool_increase(thrdpool_t *pool);
int thrdpool_decrease(thrdpool_t *pool);
void thrdpool_exit(thrdpool_t *pool);
void thrdpool_destroy(void (*pending)(const struct thrdpool_task *),
					  thrdpool_t *pool);

#ifdef __cplusplus
}
#endif

#endif

