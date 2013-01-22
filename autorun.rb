`make`
File.open('LRUZipf0.88Results.log', 'r') do |file|
	content = file.read;
	factors = content.scan(/alpha4Storage = (.*) alpha4Transport = (.*)\r\n/)
	cache_size = content.scan(/^ \[(.*)\]/)
	factors.each_with_index do |factor, index|
		File.open('factor', 'w') do |alpha|
			alpha.puts(factor[0] + " " + factor[1])
		end

		for scene in 0..2 do
			File.open('cacheSize', 'w') do |cache|
				cache.puts 2
				cache_size[index * 3 + scene][0].split(', ').each { |size| cache.puts [size.to_i, 2].max.to_s }
			end

			puts "start simulation... "
			puts `./ccnSim -u Cmdenv youtube.ini`
		end
	end
end
