# Quick Start Guide - SI3LN Docker Deployment

## 🚀 Deploy in 3 Minutes

### Development Mode (Quick Test)

```bash
cd /home/schpser/SI3LN
./deploy.sh dev
```

That's it! API runs at: http://localhost:8000/api/docs

### Production Mode

```bash
# 1. Setup secrets
cp .env.production.example .env
nano .env  # Change all SECRET values

# 2. Deploy
./deploy.sh prod
```

---

## 📦 What You Get

- ✅ **PostgreSQL Database** - Production-ready database
- ✅ **Django API** - Gunicorn + auto-restart
- ✅ **Nginx** - Reverse proxy + static files
- ✅ **Redis** - Caching layer
- ✅ **JWT Auth** - Secure with 24h expiration + pepper
- ✅ **Auto Migrations** - Database updates automatically
- ✅ **Health Checks** - Services auto-recover

---

## 🔑 Essential Commands

```bash
# Start everything
docker-compose up -d

# View logs
docker-compose logs -f

# Stop everything  
docker-compose down

# Restart API only
docker-compose restart api

# Database backup
docker-compose exec db pg_dump -U si3ln_user si3ln_db > backup.sql

# Run Django commands
docker-compose exec api python manage.py migrate
docker-compose exec api python manage.py createsuperuser
docker-compose exec api python manage.py shell

# Shell access
docker-compose exec api bash
docker-compose exec db psql -U si3ln_user si3ln_db
```

---

## 🌐 Access Points

Once deployed:

| Service | URL | Description |
|---------|-----|-------------|
| API Docs | http://localhost:8000/api/docs | Interactive API documentation |
| Admin Panel | http://localhost:8000/admin | Django admin (login: admin/admin123) |
| API Endpoints | http://localhost:8000/api/ | REST API base |
| Database | localhost:5432 | PostgreSQL (si3ln_user/DB_PASSWORD) |
| Redis | localhost:6379 | Cache server |

---

## 🔒 Production Checklist

Before deploying to production:

1. **Generate Strong Secrets**
   ```bash
   python3 -c "import secrets; print(secrets.token_urlsafe(50))"
   ```
   Use output for SECRET_KEY, JWT_SECRET_KEY, JWT_PEPPER

2. **Update .env File**
   - Set `DEBUG=False`
   - Change `DB_PASSWORD` to strong password
   - Set `ALLOWED_HOSTS` to your domain
   - Generate new secrets for all keys

3. **Configure Domain**
   - Point A record to server IP
   - Update ALLOWED_HOSTS in .env

4. **Setup HTTPS** (Recommended)
   ```bash
   sudo apt install certbot python3-certbot-nginx
   sudo certbot --nginx -d your-domain.com
   ```

5. **Firewall Rules**
   ```bash
   sudo ufw allow 80/tcp
   sudo ufw allow 443/tcp
   sudo ufw allow 22/tcp
   sudo ufw enable
   ```

---

## 🎮 Connect Your C++ Game

In your C++ code:

```cpp
// Development
APIClient apiClient("http://localhost:8000/api");

// Production
APIClient apiClient("http://your-domain.com/api");

// Or use environment variable
const char* apiUrl = std::getenv("SI3LN_API_URL");
if (!apiUrl) apiUrl = "http://localhost:8000/api";
APIClient apiClient(apiUrl);
```

---

## 🐛 Quick Troubleshooting

**API won't start?**
```bash
docker-compose logs api
docker-compose restart api
```

**Database errors?**
```bash
docker-compose logs db
docker-compose restart db
docker-compose exec api python manage.py migrate
```

**Port already in use?**
```bash
sudo lsof -i :8000  # Find what's using port
# Or change port in docker-compose.yml
```

**Clear everything and restart?**
```bash
docker-compose down -v  # Removes volumes too!
./deploy.sh dev
```

---

## 📊 Monitoring

```bash
# Service status
docker-compose ps

# Resource usage
docker stats

# Disk space
docker system df

# Cleanup old images
docker system prune -a
```

---

## 🔄 Update Deployment

```bash
# Get latest code
git pull origin main

# Rebuild and restart
docker-compose build api
docker-compose up -d api

# Run migrations
docker-compose exec api python manage.py migrate
```

---

## 📝 Environment Variables

Required in `.env`:

```env
DEBUG=False
SECRET_KEY=<50+ random chars>
JWT_SECRET_KEY=<50+ random chars>
JWT_PEPPER=<32+ random chars>
DB_PASSWORD=<strong password>
ALLOWED_HOSTS=your-domain.com,www.your-domain.com
```

Generate secrets:
```bash
python3 -c "import secrets; print('SECRET_KEY=' + secrets.token_urlsafe(50))"
python3 -c "import secrets; print('JWT_SECRET_KEY=' + secrets.token_urlsafe(50))"
python3 -c "import secrets; print('JWT_PEPPER=' + secrets.token_urlsafe(32))"
```

---

## 🎯 Testing

Test API after deployment:

```bash
# Health check
curl http://localhost/health/

# API documentation
curl http://localhost:8000/api/docs

# Register user
curl -X POST http://localhost:8000/api/auth/register \
  -H "Content-Type: application/json" \
  -d '{"username":"testuser","email":"test@test.com","password":"testpass123"}'

# Login
curl -X POST http://localhost:8000/api/auth/login \
  -H "Content-Type: application/json" \
  -d '{"username":"testuser","password":"testpass123"}'
```

---

## 📚 Full Documentation

For detailed information, see:
- [DOCKER_DEPLOYMENT.md](DOCKER_DEPLOYMENT.md) - Complete deployment guide
- [JWT_AUTH_GUIDE.md](api/JWT_AUTH_GUIDE.md) - Authentication documentation
- [README.md](README.md) - Project overview

---

## ✅ Success!

Your SI3LN game API is now fully containerized and production-ready! 🎉

Need help? Check the logs: `docker-compose logs -f`
