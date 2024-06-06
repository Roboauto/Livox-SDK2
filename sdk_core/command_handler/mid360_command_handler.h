//
// The MIT License (MIT)
//
// Copyright (c) 2022 Livox. All rights reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//

#ifndef MID360_COMMAND_HANDLER_H_
#define MID360_COMMAND_HANDLER_H_

#include <memory>
#include <map>
#include <list>
#include <mutex>

#include "base/command_callback.h"
#include "base/io_thread.h"

#include "comm/protocol.h"
#include "comm/comm_port.h"
#include "comm/define.h"

#include "livox_lidar_def.h"
#include "device_manager.h"

#include "command_handler.h"

namespace livox {
namespace lidar {  

class Mid360CommandHandler : public CommandHandler {
 public:
  explicit Mid360CommandHandler(DeviceManager* device_manager);
  ~Mid360CommandHandler() = default;
  bool Init(bool is_view) override;

  bool Init(const std::map<uint32_t, LivoxLidarCfg>& custom_lidars_cfg_map) override;
  void Handle(uint32_t handle, uint16_t lidar_port, const Command& command) override;
  void UpdateLidarCfg(const ViewLidarIpInfo& view_lidar_info) override;
  void UpdateLidarCfg(uint32_t handle, uint16_t lidar_cmd_port) override;
  livox_status SendCommand(const Command& command) override;
  livox_status SendLoggerCommand(const Command &command) override;

  static void UpdateLidarCallback(livox_status status, uint32_t handle, LivoxLidarAsyncControlResponse *response, void *client_data);
  void AddDevice(uint32_t handle);
 private:
  void SetCustomLidar(uint32_t handle, uint16_t lidar_cmd_port, const LivoxLidarCfg& lidar_cfg);
  void SetViewLidar(const ViewLidarIpInfo& view_lidar_info);
  livox_status SendCommand(const Command &command, uint16_t lidar_cmd_port);

  
  bool GetHostInfo(uint32_t handle, std::string& host_ip, uint16_t& cmd_port);

  void CommandsHandle(TimePoint now);

  void OnCommand(uint32_t handle, const Command &command);
  static void OnCommandAck(uint32_t handle, const Command &command);
  static void OnCommandCmd(uint32_t handle, uint16_t lidar_port, const Command &command);
  static bool IsStatusException(const Command &command);
  void QueryDiagnosisInfo(uint32_t handle);
  void OnLidarInfoChange(const Command &command);
 private:
  std::unique_ptr<CommPort> comm_port_;
  std::mutex device_mutex_;
  std::set<uint32_t> devices_;
  std::map<uint32_t, LivoxLidarCfg> custom_lidars_;
  bool is_view_;
};

}  // namespace livox
} // namespace lidar

#endif  // MID360_COMMAND_HANDLER_H_
