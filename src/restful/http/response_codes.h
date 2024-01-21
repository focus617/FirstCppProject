#pragma once

namespace http {

/**
 * @brief status code
 * Below is partial code list from StatusCode in httplib.h
 * Target is reduce compile time for api
*/
enum Code {
  OK = 200,
  BadRequest = 400,
  Unauthorized = 401,
  Forbidden = 403,
  NotFound = 404,
  MethodNotAllowed = 405,
  InternalServerError = 500,
  NotImplemented = 501,
};

}  // namespace http