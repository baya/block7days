f = File.new('script_opcode.txt')
tokens = []
f.readlines.each {|line|
  next if !line.include?("#define")
  token = line.split(/\s+/)
  tokens << token[1..-1]
}

ts = tokens.map {|token|
  "\"#{token[0]}\""
}.join(",")

puts "\{#{ts}\}"

ops = tokens.map {|token|
  token[1]
}.join(",")

puts "\{#{ops}\}"
