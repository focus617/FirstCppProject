#include "restful.h"

namespace http::api
{
    void list(const httplib::Request &req, httplib::Response &res)
    {
        auto numbers = req.matches[1];
        res.set_content(numbers, "text/plain");
    }

} // namespace http::api