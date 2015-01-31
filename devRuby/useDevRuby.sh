
DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )
rm $DIR/ruby
ln -s ~/.rvm/rubies/ruby-2.2.0/ $DIR/ruby