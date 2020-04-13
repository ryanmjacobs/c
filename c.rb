class C < Formula
  homepage "https://github.com/ryanmjacobs/c"
  url "https://github.com/ryanmjacobs/c/archive/v0.12.tar.gz"
  sha256 "9af9a67b8d92b77378bd679bc9274c200f2e8d07f5ed1ff459c6909f3a84a81d"
  head "https://github.com/ryanmjacobs/c.git"

  def install
    bin.install "c"
  end

  test do
    #system "#{bin}/c", "")
    pipe_output("#{bin}/c", "int main(void){return 0;}", 0)
  end
end
