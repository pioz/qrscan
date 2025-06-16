# Qrscan

A Ruby gem for decoding QR code from images.

## Installation

First install [zbar](https://zbar.sourceforge.net/) dependency.

On Debian like distro:
```
sudo apt-get install libzbar-dev
```

On Mac OSX:
```
brew install zbar
```

Then you can install the gem:
```
gem install qrscan
```

## Usage

```ruby
decoded_value = Qrscan.scan("/image/path/file.jpg")
```

## License

The gem is available as open source under the terms of the [MIT License](https://opensource.org/licenses/MIT).
