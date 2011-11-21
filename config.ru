require 'rack-legacy'

base_dir = File.expand_path("../src", __FILE__)

class IndexCgiDefault
  def initialize(app)
    @app = app
  end

  def call(env)
    request = Rack::Request.new(env)
    if request.path_info == "/"
      env["PATH_INFO"] = "/index.cgi"
    end
    @app.call(env)
  end
end

use IndexCgiDefault
use Rack::Legacy::Cgi, base_dir
run Rack::File.new base_dir
