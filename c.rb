class C < Formula
  homepage "https://github.com/ryanmjacobs/c"
  url "https://github.com/ryanmjacobs/c/archive/v0.11.tar.gz"
  sha256 "19b932e0087acf6c639cc5a4fa9a0c87314e62b0561f5517fd7a78e32fb61801"
  head "https://github.com/ryanmjacobs/c.git"

  def install
    bin.install "c"
  end

  test do
    #system "#{bin}/c", "")
    pipe_output("#{bin}/c", "int main(void){return 0;}", 0)
  end
end
