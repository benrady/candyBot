extern crate websocket;
extern crate hyper;

use std::thread;
use std::io::Write;
use websocket::{Server, Message};
use websocket::message::Type;
use hyper::Server as HttpServer;
use hyper::net::Fresh;
use hyper::server::request::Request;
use hyper::server::response::Response;

const HTML: &'static str = include_str!("web/index.html");

// The HTTP server handler
fn http_handler(_: Request, response: Response<Fresh>) {
	let mut response = response.start().unwrap();
	// Send a client webpage
	response.write_all(HTML.as_bytes()).unwrap();
	response.end().unwrap();
}

fn start() {
	// Start listening for http connections
	thread::spawn(move || {
		              let http_server = HttpServer::http("0.0.0.0:8080").unwrap();
		              http_server.handle(http_handler).unwrap();
		             });

	// Start listening for WebSocket connections
	let ws_server = Server::bind("127.0.0.1:2794").unwrap();

	for connection in ws_server.filter_map(Result::ok) {
		// Spawn a new thread for each connection.
		thread::spawn(move || {
			if !connection.protocols().contains(&"rust-websocket".to_string()) {
				connection.reject().unwrap();
				return;
			}

			let mut client = connection.use_protocol("rust-websocket").accept().unwrap();

			let ip = client.peer_addr().unwrap();

			println!("Connection from {}", ip);

			let message = Message::text("Hello".to_string());
			client.send_message(&message).unwrap();

			let (mut receiver, mut sender) = client.split().unwrap();

			for message in receiver.incoming_messages() {
				let message: Message = message.unwrap();

				match message.opcode {
					Type::Close => {
						let message = Message::close();
						sender.send_message(&message).unwrap();
						println!("Client {} disconnected", ip);
						return;
					}
					Type::Ping => {
						let message = Message::pong(message.payload);
						sender.send_message(&message).unwrap();
					}
					_ => sender.send_message(&message).unwrap(),
				}
			}
		});
	}
}

fn main() {
    println!("Starting webserver" );
    start();
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn it_works() {
        assert_eq!("Hello", "Hello");
    }
}
