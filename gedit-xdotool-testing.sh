test_file="gedit-testing.txt"

if [ -f "$test_file" ]
then
  cp /dev/null $test_file
fi

gedit -s $test_file &
gedit_pid=$!

sleep 2

WID=$(xdotool search --pid $gedit_pid | tail -1)
xdotool windowactivate --sync $WID

echo -n "test 1... "

test_text="Hello, this is the text for gedit test"
xdotool type --delay 60 --clearmodifiers "$test_text"
sleep 2
xdotool key ctrl+s
sleep 2

text_from_file=$(cat $test_file)
if [ "$text_from_file" == "$test_text" ]
then
  echo "passed"
else
  echo "failed"
fi

echo -n "test 2... "

test_string="This should be the first string in the file"
xdotool key ctrl+a
xdotool key ctrl+x
xdotool type --delay 60 --clearmodifiers "$test_string"
xdotool key Return
sleep 1
xdotool key ctrl+v
sleep 1
xdotool key ctrl+s
sleep 2

text_from_file=$(cat $test_file | tr '\n' ' ')
if [ "$text_from_file" == "$test_string $test_text" ]
then
  echo "passed"
else
  echo "failed"
fi
