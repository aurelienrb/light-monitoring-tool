#include <server_http.hpp>
#include <staticfiles.h>
#include <iostream>

typedef SimpleWeb::Server<SimpleWeb::HTTP> HttpServer;

extern const char * index_html;

std::string findStaticFile(const std::string & fullPath) {
	// remove the front '/'
	const std::string relPath(fullPath, 1);

	for (auto file : static_files::fileList()) {
		if (file.name() == relPath) {
			return file.content();
		}
	}
	return "Error 404!";
}

int main() {
	HttpServer server(8080, 1);
	server.config.address = "127.0.0.1";

	server.resource["^/$"]["GET"] = [](std::shared_ptr<HttpServer::Response> response, std::shared_ptr<HttpServer::Request> request) {
		const std::string result = index_html;
		*response << "HTTP/1.1 200 OK\r\nContent-Length: " << result.length() << "\r\n\r\n" << result;
	};

	server.resource["^/(css|js|images)/(.)+$"]["GET"] = [](std::shared_ptr<HttpServer::Response> response, std::shared_ptr<HttpServer::Request> request) {
		try {
			const std::string result = findStaticFile(request->path);
			std::cout << "Serving " << request->path << " (" << result.size() << " bytes)\n";
			//const std::string & result = findStaticFile(request->path_match[0]);
			*response << "HTTP/1.1 200 OK\r\nContent-Length: " << result.length() << "\r\n\r\n" << result;
		}
		catch (const std::exception & e) {
			std::cerr << "Error: " << e.what() << "\n";
		}
	};

	//server.resource["^/favicon.ico$"]["GET"] = [](std::shared_ptr<HttpServer::Response> response, std::shared_ptr<HttpServer::Request> request) {
	//}

	server.start();
}

const char * index_html = R"raw(

<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01//EN" "http://www.w3.org/TR/html4/strict.dtd">
<html>
<head>
	<meta http-equiv="Content-Type" content="text/html; charset=utf-8">
	<title>Light Monitor</title>
	<link rel="shortcut icon" href="" />
	<link href="css/style.css" rel="stylesheet" type="text/css">
	<!--[if lte IE 8]><script language="javascript" type="text/javascript" src="../../excanvas.min.js"></script><![endif]-->
	<script language="javascript" type="text/javascript" src="js/jquery.js"></script>
	<script language="javascript" type="text/javascript" src="js/jquery.flot.js"></script>
	<script type="text/javascript">
	$(function() {
		// We use an inline data source in the example, usually data would
		// be fetched from a server
		var data = [],
			totalPoints = 300;

		function getRandomData() {
			if (data.length > 0)
				data = data.slice(1);

			// Do a random walk
			while (data.length < totalPoints) {
				var prev = data.length > 0 ? data[data.length - 1] : 50,
					y = prev + Math.random() * 10 - 5;

				if (y < 0) {
					y = 0;
				} else if (y > 100) {
					y = 100;
				}
				data.push(y);
			}

			// Zip the generated y values with the x values
			var res = [];
			for (var i = 0; i < data.length; ++i) {
				res.push([i, data[i]])
			}
			return res;
		}

		// Set up the control widget
		var updateInterval = 30;
		$("#updateInterval").val(updateInterval).change(function () {
			var v = $(this).val();
			if (v && !isNaN(+v)) {
				updateInterval = +v;
				if (updateInterval < 1) {
					updateInterval = 1;
				} else if (updateInterval > 2000) {
					updateInterval = 2000;
				}
				$(this).val("" + updateInterval);
			}
		});

		var plot = $.plot("#placeholder", [ getRandomData() ], {
			series: {
				shadowSize: 0	// Drawing is faster without shadows
			},
			yaxis: {
				min: 0,
				max: 100
			},
			xaxis: {
				show: false
			}
		});

		function update() {
			plot.setData([getRandomData()]);
			// Since the axes don't change, we don't need to call plot.setupGrid()
			plot.draw();
			setTimeout(update, updateInterval);
		}
		update();
		// Add the Flot version string to the footer
		$("#footer").prepend("Flot " + $.plot.version + " &ndash; ");
	});
	</script>
</head>
<body>
	<div id="header">
		<h2>Real-time CPU updates</h2>
	</div>

	<div id="content">
		<div class="demo-container">
			<div id="placeholder" class="demo-placeholder"></div>
		</div>
		<p>Time between updates: <input id="updateInterval" type="text" value="" style="text-align: right; width:5em"> milliseconds</p>
	</div>
	<div id="footer">
		Copyright &copy; 2016
	</div>
</body>
</html>
)raw";