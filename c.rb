class C < Formula
  homepage "https://github.com/ryanmjacobs/c"
  url      "https://github.com/ryanmjacobs/c/archive/v0.15.0.tar.gz"
  sha256   ""
  head     "https://github.com/ryanmjacobs/c.git"
  version  "0.15.0"

  def install
    bin.install "c"
  end

  test do
    exe = File.join(bin, "c")
    pipe_output(exe, "int main(void){return 0;}", 0)
  end
end
