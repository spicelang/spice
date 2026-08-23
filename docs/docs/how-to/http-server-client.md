---
title: HTTP Server and Client
---

Spice ships a blocking HTTP/1.1 server and client in `std/net/http-server` and `std/net/http-client`, built directly
on top of the plain TCP sockets in `std/net/socket`. Both understand request/response framing via `Content-Length`
and chunked transfer encoding, so you get a working HTTP stack without pulling in any third-party dependency.

This tutorial builds a small server with a few routes, and a client that talks to it, as two separate programs -
which is how you would run them in practice.

!!! note "No TLS"
    Neither the server nor the client speaks TLS - the client rejects `https://` URLs outright. If you need to talk
    to an HTTPS endpoint, use the libcurl bindings in `std/bindings/libcurl` instead, as described in the
    [C/C++ interoperability tutorial](cpp-interop.md).

## A minimal server

Every server is built around the `HttpServer` struct. Construct it with a port, register routes on it, then `start`
it and let it `run`:

```spice
// server.spice
import "std/net/http";
import "std/net/http-server";

f<int> main() {
    HttpServer server = HttpServer(8080s);

    server.serve("/", "<h1>Hello from Spice!</h1>");

    Result<bool> started = server.start();
    if started.isErr() {
        printf("Failed to start server: %s\n", started.getErr().message);
        return 1;
    }

    printf("Listening on http://127.0.0.1:8080\n");
    Result<bool> ran = server.run();
    if ran.isErr() {
        printf("Server stopped with an error: %s\n", ran.getErr().message);
        return 1;
    }
    return 0;
}
```

`serve` registers a `GET` route that always answers with the same, fixed piece of content - handy for static pages.
`start` binds the port, and `run` then serves connections one at a time, forever, until something calls `stop` or a
connection can no longer be accepted. Run it with:

```shell
$ spice run server.spice
Listening on http://127.0.0.1:8080
```

The process keeps running in the foreground; leave it there and press ++ctrl+c++ once you are done with the
tutorial.

## A minimal client

`HttpClient` is the counterpart on the other side. Every call opens its own connection, sends one request, and
closes the connection again once the response has arrived:

```spice
// client.spice
import "std/net/http";
import "std/net/http-client";

f<int> main() {
    HttpClient client = HttpClient();

    Result<HttpResponse> result = client.get("http://127.0.0.1:8080/");
    if result.isErr() {
        printf("Request failed: %s\n", result.getErr().message);
        return 1;
    }

    HttpResponse& response = result.unwrap();
    printf("%d %s\n%s\n", response.statusCode, response.reason, response.body);
    return 0;
}
```

With the server from the previous section still running, open a second terminal and run the client:

```shell
$ spice run client.spice
200 OK
<h1>Hello from Spice!</h1>
```

## Adding routes

Besides `serve`, `HttpServer` has one method per HTTP method - `get`, `post`, `put`, `patch` and `delete` - that
takes a path and a handler lambda of type `p(const HttpRequest&, HttpResponse&)`. The handler fills in the response
that is passed to it:

```spice
server.get("/greet", p(const HttpRequest& request, HttpResponse& response) {
    String name = request.getQueryParam("name");
    if name.isEmpty() { name = String("stranger"); }

    String greeting = String("Hello, ");
    greeting.append(name);
    greeting.append('!');
    response.setBody(greeting, CONTENT_TYPE_TEXT);
});
```

`getQueryParam` reads and percent-decodes a value straight out of the query string, so a request to
`/greet?name=Spice` answers with `Hello, Spice!`, while a plain `/greet` falls back to `Hello, stranger!`.

!!! note "Routing is exact-match"
    A route only answers the exact path it was registered for - `/a` and `/a/` are different routes, path
    parameters such as `/users/{id}` are not supported, and the query string never takes part in the match. Read
    dynamic parts of the path yourself out of `request.getPath()`, and query parameters via `getQueryParam`.

A path that exists for another method answers `405 Method Not Allowed` automatically, and a `HEAD` request is
answered from the matching `GET` route with the body dropped - you don't need to register those cases yourself.

## Request and response bodies

A handler reads the request body straight off the `request.body` field, and hands a response body to
`response.setBody` together with its media type. `setJsonBody` and `setHtmlBody` are shorthands for the two most
common cases:

```spice
server.post("/users", p(const HttpRequest& request, HttpResponse& response) {
    response.setStatus(STATUS_CREATED);
    String body = String("{\"received\":");
    body.append(request.body);
    body.append('}');
    response.setJsonBody(body);
});
```

`std/net/http` defines constants for the common status codes (`STATUS_OK`, `STATUS_CREATED`, `STATUS_NOT_FOUND`,
`STATUS_INTERNAL_SERVER_ERROR`, ...) and media types (`CONTENT_TYPE_TEXT`, `CONTENT_TYPE_HTML`, `CONTENT_TYPE_JSON`,
...), so you rarely have to spell out a raw number or MIME string yourself.

## Handling unmatched requests

Without any configuration, a request that matches no route gets a plain `404`. Register a custom handler with
`setNotFoundHandler` to answer it differently:

```spice
server.setNotFoundHandler(p(const HttpRequest& request, HttpResponse& response) {
    const String path = request.getPath();
    String message = String("No route for ");
    message.append(path);
    response.setBody(message, CONTENT_TYPE_TEXT);
});
```

The status code is already set to `404` by the time your handler runs - you only need to fill in the body.

## Sending more than GET

`HttpClient` mirrors the server's set of methods - `get`, `head`, `post`, `put`, `patch` and `delete` - plus a
`request` method that takes an arbitrary `HttpMethod` for anything that does not fit the named ones. `post`, `put`
and `patch` take a body and a content type, just like `setBody` does on the server side:

```spice
String payload = String("{\"name\":\"Spice\"}");
Result<HttpResponse> created = client.post("http://127.0.0.1:8080/users", payload, CONTENT_TYPE_JSON);
if created.isErr() {
    printf("Request failed: %s\n", created.getErr().message);
    return 1;
}
HttpResponse& createdResponse = created.unwrap();
printf("%d %s\n", createdResponse.statusCode, createdResponse.body);
```

A handful of public fields on `HttpClient` let you adjust its behavior:

```spice
client.setDefaultHeader("Authorization", "Bearer secret-token"); // sent with every request
client.timeoutMillis = 5000l;  // per read/write timeout, 0 to block indefinitely
client.maxRedirects = 0u;      // hand 3xx responses back as-is instead of following them
```

`defaultHeaders` fields are only added to a request if it does not already carry a field with that name, so a
one-off request can always override them.

## Putting it all together

**`server.spice`:**

```spice
import "std/net/http";
import "std/net/http-server";

f<int> main() {
    HttpServer server = HttpServer(8080s);

    server.serve("/", "<h1>Hello from Spice!</h1>");

    server.get("/greet", p(const HttpRequest& request, HttpResponse& response) {
        String name = request.getQueryParam("name");
        if name.isEmpty() { name = String("stranger"); }

        String greeting = String("Hello, ");
        greeting.append(name);
        greeting.append('!');
        response.setBody(greeting, CONTENT_TYPE_TEXT);
    });

    server.post("/users", p(const HttpRequest& request, HttpResponse& response) {
        response.setStatus(STATUS_CREATED);
        String body = String("{\"received\":");
        body.append(request.body);
        body.append('}');
        response.setJsonBody(body);
    });

    server.setNotFoundHandler(p(const HttpRequest& request, HttpResponse& response) {
        const String path = request.getPath();
        String message = String("No route for ");
        message.append(path);
        response.setBody(message, CONTENT_TYPE_TEXT);
    });

    Result<bool> started = server.start();
    if started.isErr() {
        printf("Failed to start server: %s\n", started.getErr().message);
        return 1;
    }

    printf("Listening on http://127.0.0.1:8080\n");
    Result<bool> ran = server.run();
    if ran.isErr() {
        printf("Server stopped with an error: %s\n", ran.getErr().message);
        return 1;
    }
    return 0;
}
```

**`client.spice`:**

```spice
import "std/net/http";
import "std/net/http-client";

f<int> main() {
    HttpClient client = HttpClient();

    Result<HttpResponse> index = client.get("http://127.0.0.1:8080/");
    HttpResponse& indexResponse = index.unwrap();
    printf("index: %d %s\n", indexResponse.statusCode, indexResponse.body);

    Result<HttpResponse> greet = client.get("http://127.0.0.1:8080/greet?name=Spice");
    HttpResponse& greetResponse = greet.unwrap();
    printf("greet: %d %s\n", greetResponse.statusCode, greetResponse.body);

    String payload = String("{\"name\":\"Spice\"}");
    Result<HttpResponse> created = client.post("http://127.0.0.1:8080/users", payload, CONTENT_TYPE_JSON);
    HttpResponse& createdResponse = created.unwrap();
    printf("users: %d %s\n", createdResponse.statusCode, createdResponse.body);

    Result<HttpResponse> missing = client.get("http://127.0.0.1:8080/missing");
    HttpResponse& missingResponse = missing.unwrap();
    printf("missing: %d %s\n", missingResponse.statusCode, missingResponse.body);

    return 0;
}
```

Run the server in one terminal and the client in another:

```shell
$ spice run server.spice
Listening on http://127.0.0.1:8080
```

```shell
$ spice run client.spice
index: 200 <h1>Hello from Spice!</h1>
greet: 200 Hello, Spice!
users: 201 {"received":{"name":"Spice"}}
missing: 404 No route for /missing
```

!!! tip "Skipping error handling in a demo"
    The full example above calls `unwrap()` straight away for brevity. `unwrap()` aborts the program if the
    `Result` holds an error, so in real code check `isErr()` first, the way the earlier sections do.
