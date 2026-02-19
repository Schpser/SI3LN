# 🐳 SI3LN Docker Setup

This project uses Docker Compose to run multiple containers for easy development and deployment.

## 📦 Containers

- **PostgreSQL** (Port 5432) - Database
- **Redis** (Port 6379) - Cache & session storage
- **API Backend** (Port 8000) - Django + Ninja API
- **Frontend** (Port 80) - Nginx serving web dashboard

## 🚀 Quick Start

### Prerequisites

- Docker Engine 20.10+
- Docker Compose 2.0+

### Start All Services

```bash
cd /home/schpser/SI3LN/Docker

# Build and start all containers
docker-compose up -d

# Or build fresh (if you made changes)
docker-compose up -d --build
```

### View Logs

```bash
# All services
docker-compose logs -f

# Specific service
docker-compose logs -f api
docker-compose logs -f frontend
docker-compose logs -f db
docker-compose logs -f redis
```

### Stop Services

```bash
# Stop all containers
docker-compose down

# Stop and remove volumes (⚠️ deletes database data)
docker-compose down -v
```

## 🌐 Access Points

Once running, you can access:

- **Frontend**: http://localhost
- **API Documentation**: http://localhost:8000/api/docs
- **Admin Panel**: http://localhost:8000/admin
  - Username: `admin`
  - Password: `admin123`

## 🔧 Services Details

### Database (PostgreSQL)

```yaml
Host: localhost (or 'db' from containers)
Port: 5432
Database: si3ln_db
User: si3ln_user
Password: si3ln_password
```

### Redis Cache

```yaml
Host: localhost (or 'redis' from containers)
Port: 6379
```

### API Backend

- Built on Django + Django Ninja
- Automatic migrations on startup
- Auto-creates admin user
- Health check: `/api/game/stats`

### Frontend

- Nginx serving static files
- Proxies `/api/*` requests to backend
- Gzip compression enabled

## 🛠️ Development

### Rebuild After Code Changes

```bash
# Rebuild specific service
docker-compose build api
docker-compose build frontend

# Restart specific service
docker-compose restart api
docker-compose restart frontend
```

### Run Django Management Commands

```bash
# Create superuser
docker-compose exec api python manage.py createsuperuser

# Make migrations
docker-compose exec api python manage.py makemigrations

# Run migrations
docker-compose exec api python manage.py migrate

# Shell
docker-compose exec api python manage.py shell
```

### Access Container Shell

```bash
# API container
docker-compose exec api bash

# Database
docker-compose exec db psql -U si3ln_user -d si3ln_db

# Redis CLI
docker-compose exec redis redis-cli
```

## 📊 Database Backup & Restore

### Backup

```bash
# Backup database
docker-compose exec db pg_dump -U si3ln_user si3ln_db > backup.sql

# Backup with timestamp
docker-compose exec db pg_dump -U si3ln_user si3ln_db > "backup_$(date +%Y%m%d_%H%M%S).sql"
```

### Restore

```bash
# Restore database
cat backup.sql | docker-compose exec -T db psql -U si3ln_user si3ln_db
```

## 🔐 Environment Variables

Create `.env` file in the Docker directory for custom configuration:

```bash
# Database
POSTGRES_DB=si3ln_db
POSTGRES_USER=si3ln_user
POSTGRES_PASSWORD=your_secure_password

# Django
DEBUG=False
SECRET_KEY=your-secret-key-here
ALLOWED_HOSTS=localhost,yourdomain.com

# JWT
JWT_SECRET_KEY=your-jwt-secret
JWT_EXPIRATION_HOURS=24
```

## 🧪 Testing

```bash
# Run tests in API container
docker-compose exec api python manage.py test

# With coverage
docker-compose exec api coverage run --source='.' manage.py test
docker-compose exec api coverage report
```

## 📈 Monitoring

### Check Container Status

```bash
docker-compose ps
```

### Resource Usage

```bash
docker stats
```

### Health Checks

```bash
# API health
curl http://localhost:8000/api/game/stats

# Database
docker-compose exec db pg_isready -U si3ln_user

# Redis
docker-compose exec redis redis-cli ping
```

## 🚨 Troubleshooting

### Port Already in Use

```bash
# Check what's using port 80
sudo lsof -i :80

# Check what's using port 8000
sudo lsof -i :8000

# Stop conflicting service or change port in docker-compose.yml
```

### Database Connection Issues

```bash
# Check database logs
docker-compose logs db

# Verify database is healthy
docker-compose ps

# Restart database
docker-compose restart db
```

### Reset Everything

```bash
# ⚠️ WARNING: This deletes ALL data
docker-compose down -v
docker-compose up -d --build
```

## 📝 Production Deployment

For production, update:

1. Change all passwords and secrets
2. Set `DEBUG=False`
3. Configure proper `ALLOWED_HOSTS`
4. Use environment variables for sensitive data
5. Set up proper volume backups
6. Configure SSL/TLS certificates
7. Use a reverse proxy (Caddy, Traefik, or Nginx)
8. Set up monitoring and logging

## 🎯 Next Steps

- [ ] Set up CI/CD pipeline
- [ ] Add monitoring (Prometheus + Grafana)
- [ ] Configure log aggregation
- [ ] Set up automated backups
- [ ] Add rate limiting
- [ ] Implement SSL certificates

---

**Made with ❤️ by hugou74130 & Schpser**
