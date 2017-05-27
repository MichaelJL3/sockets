# README.txt contains instructions how to build and run your code.

# TODO: document how to invoke protoc in order to generate the stubs in your

(make sure you have grpc and grpc tools installed)
(pip install grpcio)
(pip install grpcio-tools)

python -m grpc.tools.protoc -I. --python_out=. --grpc_python_out=. debate.proto

python -m grpc.tools.protoc -I. --python_out=. --grpc_python_out=. consultation.proto

(I kept all the files together in the same directory)

# language of choice.
python (code written in 2.7)

# YOUR INSTRUCTIONS GO HERE
install the dependencies listed above ^

run this command to compile the proto file

python -m grpc.tools.protoc -I. --python_out=. --grpc_python_out=. debate.proto

# TODO: document how to build your server and client code (if applicable)
# YOUR INSTRUCTIONS GO HERE

python server.py

for the client it looks for a minimum of 2 arguments or it will print an error and exit

for answer the words answer or Answer both work 
for elaborate the wrods elaborate and Elaborate both work

with elaborate if no number is provided it just prints the topic
as specified in the assignment

also any non numerical entry for the array is ignored and the program
continues bu will provide the user with a warning message

to run the client:

python moderator.py answer "blah blah blah"
python moderator.py elaborate "blah blah blah" 1 2 3 4

# TODO: document how run your server (on localhost)
# YOUR INSTRUCTIONS GO HERE

python server.py

**Note
Also wraps the all the rpc calls to servers with a try/catch block
in case theres an error connecting, will display error and return an empty string if it occurs

Also displays warning if the the connection succeeds but nothing is returned from the server

Also note port is set to 50000 however if a cmd arg is entered when running server exampl

python server.py 50001 

the port will be set to 50001 however you would still have to manually change the clients
port to 50001

also git says i changed the consultation file I did not make any actual changes i moved it around between OS's and maybe copied stuff from it but the original code has not been modified in anyway
