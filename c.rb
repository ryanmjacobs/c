class C < Formula
  homepage "https://github.com/ryanmjacobs/c"
  url      "https://github.com/ryanmjacobs/c/archive/refs/tags/v0.15.1.tar.gz"
  sha256   "ecfad78cb0ab56da44dcfed805f5c261ddefd6dc4a4e57eb2dcfcffa85330605"
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
