from http.server import BaseHTTPRequestHandler, ThreadingHTTPServer
from pathlib import Path
from mimetypes import guess_type


ROOT = Path(__file__).resolve().parent


class PreviewHandler(BaseHTTPRequestHandler):
    def do_GET(self):
        target = ROOT / "showcase.html" if self.path in {"/", "/showcase.html"} else ROOT / self.path.lstrip("/")

        if not target.exists() or not target.is_file():
            self.send_error(404, "File not found")
            return

        content_type = guess_type(target.name)[0] or "application/octet-stream"
        data = target.read_bytes()
        self.send_response(200)
        self.send_header("Content-Type", content_type)
        self.send_header("Content-Length", str(len(data)))
        self.end_headers()
        self.wfile.write(data)

    def log_message(self, format, *args):
        return


def main():
    server = ThreadingHTTPServer(("127.0.0.1", 8000), PreviewHandler)
    print("Preview server running at http://127.0.0.1:8000")
    print("Press Ctrl+C to stop.")
    server.serve_forever()


if __name__ == "__main__":
    main()