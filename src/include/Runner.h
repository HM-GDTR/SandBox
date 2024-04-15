//
// Created by RainbowX on 24-3-28.
//

#ifndef SANDBOX_RUNNER_H
#define SANDBOX_RUNNER_H

#include <cassert>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/resource.h>

#include "types.hpp"
#include "status.hpp"

SandBoxStatus runProblem(ProcessInfo info, SysCallRestricts restricts);
SandBoxStatus runSample(ProcessInfo info, SysCallRestricts restricts, size_t example_idx);

#endif //SANDBOX_RUNNER_H
