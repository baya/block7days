#!/usr/bin/env ruby
# str = "18E14A7B6A307F426A94F8114701E7C8E774E7F9A47E2C2035DB29A206321725"
str = ARGV[0]

tokens_list = []
count = 0
bstr_list = str.scan(/../).map {|tkn| "0x#{tkn}" }.each_with_index {|item, i|

  tokens_list[count] ||= []
  tokens_list[count] << item

  if (i+1) % 8 == 0
    count += 1
  end

}

tstr = tokens_list.map {|tokens|
  tokens.join(",")
}.join(",\n")


puts tstr

