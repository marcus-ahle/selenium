// Copyright 2011 WebDriver committers
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef WEBDRIVER_IE_GETELEMENTATTRIBUTECOMMANDHANDLER_H_
#define WEBDRIVER_IE_GETELEMENTATTRIBUTECOMMANDHANDLER_H_

#include "../Browser.h"
#include "../IECommandHandler.h"
#include "../IECommandExecutor.h"

namespace webdriver {

class GetElementAttributeCommandHandler : public IECommandHandler {
public:
	GetElementAttributeCommandHandler(void) {
	}

	virtual ~GetElementAttributeCommandHandler(void) {
	}

protected:
	void GetElementAttributeCommandHandler::ExecuteInternal(const IECommandExecutor& executor, const LocatorMap& locator_parameters, const ParametersMap& command_parameters, Response * response) {
		LocatorMap::const_iterator id_parameter_iterator = locator_parameters.find("id");
		LocatorMap::const_iterator name_parameter_iterator = locator_parameters.find("name");
		if (id_parameter_iterator == locator_parameters.end()) {
			response->SetErrorResponse(400, "Missing parameter in URL: id");
			return;
		} else if (name_parameter_iterator == locator_parameters.end()) {
			response->SetErrorResponse(400, "Missing parameter in URL: name");
			return;
		} else {
			std::string element_id = id_parameter_iterator->second;
			std::string name = name_parameter_iterator->second;

			BrowserHandle browser_wrapper;
			int status_code = executor.GetCurrentBrowser(&browser_wrapper);
			if (status_code != SUCCESS) {
				response->SetErrorResponse(status_code, "Unable to get browser");
				return;
			}

			ElementHandle element_wrapper;
			status_code = this->GetElement(executor, element_id, &element_wrapper);
			if (status_code == SUCCESS) {
				std::string value = "";
				bool is_null;
				status_code = element_wrapper->GetAttributeValue(name, &value, &is_null);
				if (status_code != SUCCESS) {
					response->SetErrorResponse(status_code, "Unable to get attribute");
					return;
				} else {
					if (is_null) {
						response->SetSuccessResponse(Json::Value::null);
						return;
					} else {
						response->SetSuccessResponse(value);
						return;
					}
				}
			} else {
				response->SetErrorResponse(status_code, "Element is no longer valid");
				return;
			}
		}
	}
};

} // namespace webdriver

#endif // WEBDRIVER_IE_GETELEMENTATTRIBUTECOMMANDHANDLER_H_
