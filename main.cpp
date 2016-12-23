#include "server.h"

int main() {
	Server echo_server(8000);
	echo_server.run();
}
