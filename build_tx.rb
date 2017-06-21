require 'bitcoin'

key = Bitcoin::generate_key

puts key.inspect

address = Bitcoin::pubkey_to_address(key[1])

puts address