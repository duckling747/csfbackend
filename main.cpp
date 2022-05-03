#include "crow.h"
#include "crow/mustache.h"
#include "crow/compression.h"
#include <cstdlib>

#define CROW_ENFORCE_WS_SPEC
#define CROW_JSON_USE_MAP


static int get_port (void) {
	#ifdef _MSC_VER
		char p [7];
		size_t len;
		getenv_s(&len, p, 7, "PORT");
    	return len ? std::atoi(p) : 8080;
	#else
		const char * p = getenv("PORT");
		return p ? std::stoi(p) : 8080;
	#endif
}

static const crow::mustache::template_t templates [3] = {
	crow::mustache::load("index.html"),
	crow::mustache::load("about.html"),
	crow::mustache::load("help.html")
};

int main(void) {
        
	static crow::SimpleApp app;

	CROW_ROUTE(app, "/")
		.methods(crow::HTTPMethod::GET)
	(
		[]() {
			return templates[0].render();
		}
	);
	CROW_ROUTE(app, "/about")
		.methods(crow::HTTPMethod::GET)
		(
			[]() {
				return templates[1].render();
			}
	);
	CROW_ROUTE(app, "/help")
		.methods(crow::HTTPMethod::GET)
		(
			[]() {
				return templates[2].render();
			}
	);
	CROW_ROUTE(app, "/manifest.json")
		.methods(crow::HTTPMethod::GET)
		(
			[](crow::response& r) {
				r.set_static_file_info("manifest.json");
				return r.end();
			}
	);

	app.port(get_port())
		.use_compression(crow::compression::algorithm::DEFLATE)
		.loglevel(crow::LogLevel::Warning)
		.multithreaded()
		.run();
}
