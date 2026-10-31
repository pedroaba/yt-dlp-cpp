#pragma once

namespace vidx {
// Registered codes: https://www.iana.org/assignments/http-status-codes/
enum class HttpStatusCode : long {
  // 1xx: Informational
  Continue = 100, // Headers accepted; the client may send the request body.
  SwitchingProtocols =
      101, // The server is switching to the requested protocol.
  Processing =
      102, // The request is still being processed; no final response yet.
  EarlyHints =
      103, // Preliminary headers allow the client to prepare resources early.
  UploadResumptionSupported =
      104, // The server supports resuming uploads; temporary registration.

  // 2xx: Success
  Ok = 200,       // The request succeeded.
  Created = 201,  // The request created one or more resources.
  Accepted = 202, // Accepted for processing, but processing is not complete.
  NonAuthoritativeInformation =
      203, // A transforming proxy modified the successful response content.
  NoContent = 204, // The request succeeded with no response content.
  ResetContent =
      205, // The request succeeded; the client should reset its document view.
  PartialContent =
      206,           // The response contains the requested range of a resource.
  MultiStatus = 207, // The WebDAV response reports separate results for
                     // multiple resources.
  AlreadyReported =
      208,      // WebDAV binding members were already listed in this response.
  ImUsed = 226, // The response contains results of instance manipulations, such
                // as a delta.

  // 3xx: Redirection
  MultipleChoices =
      300, // Multiple representations are available for the requested resource.
  MovedPermanently = 301, // The resource has permanently moved to another URI.
  Found = 302,            // The resource temporarily resides at another URI.
  SeeOther = 303, // Retrieve the result from another URI using GET or HEAD.
  NotModified =
      304, // The cached representation is still valid; no content is sent.
  UseProxy = 305,  // Deprecated code that directed clients to use a proxy.
  Unused306 = 306, // Reserved code that is no longer used.
  TemporaryRedirect =
      307, // Temporary redirect that preserves the request method and body.
  PermanentRedirect =
      308, // Permanent redirect that preserves the request method and body.

  // 4xx: Client errors
  BadRequest =
      400, // The server cannot process the request due to a client error.
  Unauthorized =
      401, // Valid authentication credentials are required for this resource.
  PaymentRequired =
      402, // Reserved for future use; payment semantics are not standardized.
  Forbidden =
      403, // The server understood the request but refuses to fulfill it.
  NotFound =
      404, // The resource was not found, or the server will not disclose it.
  MethodNotAllowed =
      405,             // The request method is not supported by this resource.
  NotAcceptable = 406, // No available representation satisfies the content
                       // negotiation headers.
  ProxyAuthenticationRequired =
      407, // Authentication with the proxy is required.
  RequestTimeout =
      408, // The server timed out waiting for the complete request.
  Conflict =
      409,    // The request conflicts with the current state of the resource.
  Gone = 410, // The resource is no longer available, likely permanently.
  LengthRequired =
      411, // The request must include a valid Content-Length header.
  PreconditionFailed =
      412, // A condition in the request headers evaluated to false.
  ContentTooLarge =
      413, // The request content exceeds what the server is willing to process.
  UriTooLong = 414, // The request URI is longer than the server will accept.
  UnsupportedMediaType =
      415, // The request content format or encoding is not supported.
  RangeNotSatisfiable =
      416, // The requested ranges cannot be satisfied for this resource.
  ExpectationFailed =
      417, // The server cannot meet the requirement in the Expect header.
  Unused418 = 418, // Reserved and unused in current HTTP semantics.
  MisdirectedRequest =
      421, // The request reached a server unable to serve the target URI.
  UnprocessableContent = 422, // The content type and syntax are valid, but its
                              // instructions cannot be processed.
  Locked = 423, // The WebDAV source or destination resource is locked.
  FailedDependency =
      424, // The operation failed because a required dependent action failed.
  TooEarly = 425, // The server refuses early data because the request could be
                  // replayed.
  UpgradeRequired =
      426, // The client must switch protocols before the request can succeed.
  PreconditionRequired =
      428, // A conditional request is required to avoid conflicting updates.
  TooManyRequests =
      429, // The client sent too many requests within a given time.
  RequestHeaderFieldsTooLarge =
      431, // One or more request header fields are too large.
  UnavailableForLegalReasons =
      451, // Access to the resource is denied for legal reasons.

  // 5xx: Server errors
  InternalServerError =
      500, // An unexpected server condition prevented completion.
  NotImplemented =
      501, // The server does not support functionality required by the request.
  BadGateway = 502, // A gateway or proxy received an invalid response from an
                    // upstream server.
  ServiceUnavailable = 503, // The server is temporarily unavailable, often due
                            // to overload or maintenance.
  GatewayTimeout =
      504, // A gateway or proxy timed out waiting for an upstream server.
  HttpVersionNotSupported =
      505, // The server does not support the HTTP version used in the request.
  VariantAlsoNegotiates = 506, // Content negotiation is misconfigured because a
                               // variant also negotiates.
  InsufficientStorage =
      507,            // The server lacks storage to complete the operation.
  LoopDetected = 508, // The server detected an infinite loop while processing a
                      // WebDAV request.
  NotExtended = 510, // Obsolete code for an HTTP extension requirement that was
                     // not satisfied.
  NetworkAuthenticationRequired =
      511, // Network access requires authentication, such as through a captive
           // portal.
};
} // namespace vidx