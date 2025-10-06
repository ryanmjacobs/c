class C < Formula
  homepage "https://github.com/ryanmjacobs/c"
  url      "https://github.com/ryanmjacobs/c/archive/v0.14.tar.gz"
  sha256   "2b66d79d0d5c60b8e6760dac734b8ec9a7d6a5e57f033b97086821b1985a870b"
  head     "https://github.com/ryanmjacobs/c.git"
  version  "0.14"

  def install
    bin.install "c"
  end

  test do
    exe = File.join(bin, "c")
    pipe_output(exe, "int main(void){return 0;}", 0)
  end
end
