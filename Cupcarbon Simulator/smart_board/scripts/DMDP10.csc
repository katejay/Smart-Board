//Receiver DMD
loop
print Date
//wait
read v
if($v==A)
	print Value_From_Air_Pressure
	break
else if($v==B)
	print Value_From_Air_Quality
	break
end