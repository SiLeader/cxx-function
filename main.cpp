//   Copyright 2018 SiLeader and Cerussite.
//
//   Licensed under the Apache License, Version 2.0 (the "License");
//   you may not use this file except in compliance with the License.
//   You may obtain a copy of the License at
//
//       http://www.apache.org/licenses/LICENSE-2.0
//
//   Unless required by applicable law or agreed to in writing, software
//   distributed under the License is distributed on an "AS IS" BASIS,
//   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//   See the License for the specific language governing permissions and
//   limitations under the License.

#include "function.hpp"

#include <iostream>

namespace {
    struct f_obj {
        void operator()(int n)const {
            std::cout<<"function object "<<n<<std::endl;
        }
    };
    void f(int n) {
        std::cout<<"function "<<n<<std::endl;
    }
} /* anonymouse */

int main() {
    func::function<void(int)> vif;

    vif=[](int n){
        std::cout<<"lambda "<<n<<std::endl;
    };
    vif(2);

   vif = f_obj{};
   vif(3);

   vif = f;
   vif(4);

   return 0;
}
