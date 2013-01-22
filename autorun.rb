`make`
File.open('LRUZipf0.88Results.log', 'r') do |file|
	content = file.read;
	factors = content.scan(/alpha4Storage = (.*) alpha4Transport = (.*)\r\n/)
	cache_size = content.scan(/^ \[(.*)\]/)
	factors.each_with_index do |factor, index|
		File.open('factor', 'w') do |alpha|
			alpha.puts(factor[0] + " " + factor[1])
		end

		puts cache_size[index * 3 + 2][0]
		next if cache_size[index * 3 + 2][0] == "99999, 99999, 99999, 99999, 99999, 99999, 99999, 99999, 99999, 99999, 99999, 99999, 99999, 99999"
		File.open('cacheSize', 'w') do |cache|
			cache.puts 2
			cache_size[index * 3 + 2][0].split(', ').each { |size| cache.puts ([size.to_i, 2].max * 10).to_s }
		end

		puts "start simulation... "
		`./ccnSim -u Cmdenv youtube.ini`

	end
end
