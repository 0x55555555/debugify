
module NotifierExtensions
  def listen(&x)
    super(x)
  end

  def self.install(clss)
    clss.each do |n|
      n.prepend(NotifierExtensions)
    end
  end
end