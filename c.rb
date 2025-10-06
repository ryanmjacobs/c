class C < Formula
  homepage "https://github.com/ryanmjacobs/c"
  url      "https://github.com/ryanmjacobs/c/archive/refs/tags/v0.15.1.tar.gz"
  sha256   "e07ce163412ccf5163d3d974a19cd768bc289f3cacdf2978dcf908682f51a07a"
  head     "https://github.com/ryanmjacobs/c.git"
  version  "0.15.1"

  def install
    bin.install "c"
  end

  test do
    exe = File.join(bin, "c")
    pipe_output(exe, "int main(void){return 0;}", 0)
  end
end
