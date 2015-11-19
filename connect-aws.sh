# Connect to AWS Instance
# ssh -i /Users/aaronsheah/Documents/Keys/ec2-keypair.pem admin@54.165.177.175 ## GPU Instance
ssh -i /Users/aaronsheah/Documents/Keys/ec2-keypair.pem ubuntu@54.236.150.88 ## Free Instance

# scp -i /Users/aaronsheah/Documents/Keys/ec2-keypair.pem provider/puzzles.cpp admin@54.165.177.175:hpce-2015-cw5_ajs212/provider/puzzles.cpp
# scp -i /Users/aaronsheah/Documents/Keys/ec2-keypair.pem provider/makefile admin@54.165.177.175:hpce-2015-cw5_ajs212/provider/makefile

# scp -i /Users/aaronsheah/Documents/Keys/ec2-keypair.pem provider/user_brute_force.hpp admin@54.165.177.175:hpce-2015-cw5_ajs212/provider/user_brute_force.hpp
# scp -i /Users/aaronsheah/Documents/Keys/ec2-keypair.pem provider/user_brute_force_kernel.cl admin@54.165.177.175:hpce-2015-cw5_ajs212/provider/user_brute_force_kernel.cl

# scp -i /Users/aaronsheah/Documents/Keys/ec2-keypair.pem /.ssh/id_dsa admin@54.175.231.178:~/.ssh/id_dsahttps://github.com/settings/notifications