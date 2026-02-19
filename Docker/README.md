# SI3LN API - Docker Setup

This directory contains everything needed to run the SI3LN Django API in Docker containers.

## 📋 Prerequisites

- Docker (20.10+)
- Docker Compose (1.29+)

## 🚀 Quick Start

### 1. Build and Start the API

From the `Docker` directory:

```bash
cd /home/schpser/SI3LN/Docker
docker-compose up --build
```

The API will be available at: **http://localhost:8000**

### 2. Access the API

- **API Documentation**: http://localhost:8000/api/docs
- **Admin Panel**: http://localhost:8000/admin
  - Username: `admin`
  - Password: `admin123`

### 3. Stop the API

```bash
docker-compose down
```

To also remove volumes (database data):
```bash
docker-compose down -v
```

## 📁 Project Structure

```
Docker/
├── Dockerfile           # Container image definition
├── docker-compose.yml   # Service orchestration
├── .dockerignore       # Files to exclude from image
├── .env.example        # Environment variables template
└── README.md           # This file

api/
├── entrypoint.sh       # Container startup script
├── manage.py           # Django management
├── requirements.txt    # Python dependencies
└── si3ln_api/         # Django project
```

## 🔧 Configuration

### Environment Variables

Create a `.env` file in the Docker directory (optional):

```bash
cp .env.example .env
# Edit .env with your settings
```

### Using PostgreSQL Instead of SQLite

1. Uncomment the `db` service in `docker-compose.yml`
2. Update the `DATABASE_URL` environment variable
3. Rebuild: `docker-compose up --build`

## 📝 Common Commands

### View Logs
```bash
docker-compose logs -f api
```

### Run Django Commands
```bash
# Create migrations
docker-compose exec api python manage.py makemigrations

# Run migrations
docker-compose exec api python manage.py migrate

# Create superuser
docker-compose exec api python manage.py createsuperuser

# Django shell
docker-compose exec api python manage.py shell
```

### Access Container Shell
```bash
docker-compose exec api bash
```

### Rebuild Image
```bash
docker-compose build --no-cache
docker-compose up
```

## 🧪 Testing the API

### Test Endpoints
```bash
# Get game stats (public endpoint)
curl http://localhost:8000/api/game/stats

# Register a user
curl -X POST http://localhost:8000/api/auth/register \
  -H "Content-Type: application/json" \
  -d '{"username":"testuser","password":"testpass123","email":"test@example.com"}'

# Login
curl -X POST http://localhost:8000/api/auth/login \
  -H "Content-Type: application/json" \
  -d '{"username":"testuser","password":"testpass123"}'
```

## 🔐 Security Notes

**For Production:**

1. Change the admin password immediately
2. Set `DJANGO_DEBUG=False` in environment
3. Update `SECRET_KEY` to a secure random value
4. Configure `ALLOWED_HOSTS` properly
5. Use PostgreSQL instead of SQLite
6. Enable HTTPS/SSL
7. Use proper secrets management (not .env files)

## 🛠️ Troubleshooting

### Port 8000 Already in Use
```bash
# Find and kill the process
lsof -ti :8000 | xargs kill -9

# Or use a different port in docker-compose.yml
ports:
  - "8001:8000"
```

### Database Errors
```bash
# Reset database
docker-compose down -v
docker-compose up --build
```

### Permission Issues
```bash
# Fix permissions on mounted volumes
sudo chown -R $USER:$USER ../api/Base_de_donnée_SQL
sudo chown -R $USER:$USER ../api/media
sudo chown -R $USER:$USER ../api/staticfiles
```

## 📚 API Routes

See [API_ROUTES.md](../api/API_ROUTES.md) for complete endpoint documentation.

## 🎯 Health Check

The container includes a health check that pings `/api/game/stats` every 30 seconds.

Check container health:
```bash
docker-compose ps
```

## 📦 Volumes

Data persisted in volumes:
- `Base_de_donnée_SQL/` - SQLite database
- `media/` - Uploaded media files
- `staticfiles/` - Collected static files

## 🌐 Network

The API runs in a Docker network named `si3ln_default` (auto-created).

---

**Need help?** Check the logs: `docker-compose logs -f api`
