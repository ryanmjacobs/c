class C < Formula
  homepage "https://github.com/ryanmjacobs/c"
  url "https://github.com/ryanmjacobs/c/archive/v0.11.tar.gz"
  sha256 "invalid"

  def install
    bin.install "c"
  end

  test do
    #system "#{bin}/c", "")
    pipe_output("#{bin}/c", "int main(void){return 0;}", 0)
  end
end
