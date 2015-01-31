
DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )
rm $DIR/ruby
ln -s $DIR/../build/Resources/ruby $DIR/ruby