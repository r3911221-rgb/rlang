def main()
  let name = "R Language"
  let version = 0.1
  
  if version > 0
    puts("Hello, " + name + "!")
  end
  
  case version
  when 0
    "zero"
  when 0.1
    "point one"
  else
    "other"
  end
  
  for i in 0..10
    puts(i)
  end
  
  return 0
end
