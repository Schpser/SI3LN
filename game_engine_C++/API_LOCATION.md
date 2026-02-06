# API Location

The Django Ninja API for this game engine is located at:

```
/home/schpser/SI3LN/api/
```

## Why is the API separate?

The API is kept outside the game_engine_C++ directory to maintain clear separation of concerns:

- **game_engine_C++/** - Core game engine written in C++
- **api/** - Django Ninja REST API for game data management
- **web_dashboard/** - Frontend dashboard
- **infrastructure/** - Deployment and infrastructure configuration

## Using the API

To start the API server:

```bash
cd ../api
python manage.py runserver
```

Or use the convenience script:

```bash
cd ../api
chmod +x start.sh
./start.sh
```

## API Client Examples

Client examples for integrating with the API can be found in:
- Python: `../api/examples/example_client.py`
- C++: `../api/examples/api_client_example.cpp`

## Documentation

Full API documentation is available at:
- README: `../api/README.md`
- Interactive API docs: http://localhost:8000/api/docs (when server is running)
