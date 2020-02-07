/*
 * Copyright (C) 2019 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <gtest/gtest.h>

#include "linkerconfig/configwriter.h"
#include "linkerconfig/variables.h"

constexpr const char* kSampleContent = "Lorem ipsum dolor sit amet";

constexpr const char* kExpectedResultWithVariables =
    R"(/Value/Test
/Invalid_Value/Path
//Path2
)";

TEST(linkerconfig_configwriter, write_line) {
  android::linkerconfig::modules::ConfigWriter writer;

  writer.WriteLine(kSampleContent);

  ASSERT_EQ(writer.ToString(), "Lorem ipsum dolor sit amet\n");
}

TEST(linkerconfig_configwriter, replace_variable) {
  android::linkerconfig::modules::ConfigWriter writer;

  android::linkerconfig::modules::Variables::AddValue("Test_Prop_Q", "Value");
  android::linkerconfig::modules::Variables::AddValue("VNDK_VER", "Q");

  writer.WriteLine("/@{Test_Prop_@{VNDK_VER}}/Test");
  writer.WriteLine("/@{Invalid_Key:Invalid_Value}/Path");
  writer.WriteLine("/@{Invalid_Key:}/Path2");

  ASSERT_EQ(writer.ToString(), kExpectedResultWithVariables);
}

TEST(linkerconfig_configwriter, WriteVars) {
  android::linkerconfig::modules::ConfigWriter writer;

  writer.WriteVars("var1", {"value1", "value2"});
  writer.WriteVars("var2", {"value1"});
  writer.WriteVars("var3", {});

  ASSERT_EQ(
      "var1 = value1\n"
      "var1 += value2\n"
      "var2 = value1\n",
      writer.ToString());
}