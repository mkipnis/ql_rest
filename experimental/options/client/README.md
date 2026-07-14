

python3 -m venv .venv
source .venv/bin/activate

pip install pip-tools
pip-compile requirements.in

pip install -r requirements.txt
