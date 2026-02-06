# SI3LN - Space Invaders 3D League Network

Complete C++ game with Django REST API backend, JWT authentication, and Docker deployment.

## 🎮 Project Structure

```
SI3LN/
├── api/                      # Django REST API
│   ├── game/                 # Game app with models & endpoints
│   ├── si3ln_api/           # Django project settings
│   ├── Dockerfile           # API container
│   └── requirements.txt     # Python dependencies
├── game_engine_C++/         # C++ Game Engine
│   ├── src/                 # Source code
│   ├── include/             # Headers
│   ├── CMakeLists.txt       # Build configuration
│   └── Dockerfile           # Game container (optional)
├── infrastructure/          # Deployment configs
│   └── nginx/               # Nginx configuration
├── docker-compose.yml       # Orchestration
└── deploy.sh               # Deployment script
```

## 🚀 Quick Start

### Start API Server

```bash
# Quick start (automated)
cd /home/schpser/SI3LN
./quick-test-no-docker.sh

# API available at: http://localhost:8000/api/docs
```

### Or Manual Setup

```bash
# Terminal 1: Start API
cd api
python3 -m venv venv
source venv/bin/activate
pip install -r requirements.txt
python manage.py migrate
python manage.py runserver

# Terminal 2: Build & Run Game
cd game_engine_C++
./build.sh
./build/SI3LN
```

## 📦 Features

### API (Django Ninja)
- ✅ RESTful endpoints for game data
- ✅ JWT authentication with HMAC-SHA256 pepper
- ✅ 24-hour token expiration
- ✅ Player profiles & game sessions
- ✅ Leaderboard & statistics
- ✅ OpenAPI documentation
- ✅ CORS enabled for game clients

### Game Engine (C++)
- ✅ SFML graphics
- ✅ HTTP API client with libcurl
- ✅ JSON parsing with nlohmann-json
- ✅ Automatic player registration/login
- ✅ Session management
- ✅ Leaderboard integration

### Development Setup
- ✅ SQLite database (development)
- ✅ Django development server
- ✅ Hot reload on code changes
- ✅ Easy local testing

## 🔒 Security

- **JWT Tokens**: HS256 algorithm with 24-hour expiration
- **Pepper Enhancement**: HMAC-SHA256 hashing for extra security
- **Protected Endpoints**: Authentication required for sensitive operations
- **Public Endpoints**: Leaderboard and stats accessible without auth
- **HTTPS Ready**: Nginx configured for SSL/TLS

## 🌐 API Endpoints

### Authentication
- `POST /api/auth/register` - Register new player
- `POST /api/auth/login` - Login and get JWT token
- `POST /api/auth/refresh` - Refresh token
- `POST /api/auth/logout` - Logout (invalidate token)
- `GET /api/auth/me` - Get current user info

### Game Operations (Protected)
- `GET /api/game/players` - List all players
- `POST /api/game/players` - Create player
- `GET /api/game/players/{id}` - Get player details
- `PUT /api/game/players/{id}` - Update player
- `DELETE /api/game/players/{id}` - Delete player
- `POST /api/game/sessions` - Start game session
- `PUT /api/game/sessions/{id}` - Update session
- `DELETE /api/game/sessions/{id}` - End session

### Public Endpoints
- `GET /api/game/leaderboard` - Get top players
- `GET /api/game/stats` - Get game statistics

## 🔧 Configuration

### Environment Variables

Create `.env` file (use `.env.production.example` as template):

```env
DEBUG=False
SECRET_KEY=<generate-with-secrets-script>
JWT_SECRET_KEY=<generate-with-secrets-script>
JWT_PEPPER=<generate-with-secrets-script>
JWT_EXPIRATION_HOURS=24
DB_PASSWORD=<strong-password>
ALLOWED_HOSTS=your-domain.com,localhost
```

Generate secrets:
```bash
./generate-secrets.sh
```

### C++ Game Configuration

Update API URL in your game:

```cpp
// In main.cpp or Game.cpp
const char* apiUrl = std::getenv("SI3LN_API_URL");
if (!apiUrl) apiUrl = "http://localhost:8000/api";
APIClient apiClient(apiUrl);
```

## 📚 Documentation

- [QUICKSTART.md](QUICKSTART.md) - Get started in 3 minutes
- [DOCKER_DEPLOYMENT.md](DOCKER_DEPLOYMENT.md) - Complete Docker guide
- [JWT_AUTH_GUIDE.md](api/JWT_AUTH_GUIDE.md) - Authentication details
- [JWT_IMPLEMENTATION_SUMMARY.md](api/JWT_IMPLEMENTATION_SUMMARY.md) - Quick reference

## 🛠️ Development

### API Development

```bash
cd api
source venv/bin/activate

# Run tests
python manage.py test

# Create migrations
python manage.py makemigrations

# Apply migrations
python manage.py migrate

# Create superuser
python manage.py createsuperuser

# Run development server
python manage.py runserver
```

### C++ Development

```bash
cd game_engine_C++

# Install dependencies (Ubuntu/Debian)
sudo ./install_api_deps.sh

# Build
./build.sh

# Run
./build/SI3LN

# Clean build
rm -rf build && ./build.sh
```

## �️ Development Commands

```bash
# Start API server
./quick-test-no-docker.sh

# Or manually:
cd api
source venv/bin/activate
python manage.py runserver

# Run migrations
python manage.py migrate

# Create superuser
python manage.py createsuperuser

# Django shell
python manage.py shell

# Run tests
python manage.py test

# Database backup (SQLite)
cp db.sqlite3 backup_$(date +%Y%m%d).sqlite3
```

## 🧪 Testing

### API Tests

```bash
cd api
python test_api.py
```

### Manual Testing

```bash
# Register user
curl -X POST http://localhost:8000/api/auth/register \
  -H "Content-Type: application/json" \
  -d '{"username":"player1","email":"player1@game.com","password":"pass123"}'

# Login (get token)
curl -X POST http://localhost:8000/api/auth/login \
  -H "Content-Type: application/json" \
  -d '{"username":"player1","password":"pass123"}'

# Use token for protected endpoint
TOKEN="your-jwt-token-here"
curl http://localhost:8000/api/game/players \
  -H "Authorization: Bearer $TOKEN"

# Public endpoints (no auth needed)
curl http://localhost:8000/api/game/leaderboard
curl http://localhost:8000/api/game/stats
```

## 🌍 Production Deployment

### 1. Server Setup

```bash
# Install Docker & Docker Compose
curl -fsSL https://get.docker.com | sh
sudo usermod -aG docker $USER

# Clone repository
git clone <your-repo> /opt/si3ln
cd /opt/si3ln
```

### 2. Configure

```bash
# Generate secrets
./generate-secrets.sh

# Create .env
cp .env.production.example .env
nano .env  # Update all values
```

### 3. Deploy

```bash
./deploy.sh prod
```

### 4. Setup Domain & HTTPS

```bash
# Point DNS A record to server IP
# Install Certbot
sudo apt install certbot python3-certbot-nginx

# Get SSL certificate
sudo certbot --nginx -d your-domain.com -d www.your-domain.com

# Auto-renew
sudo certbot renew --dry-run
```

### 5. Firewall

```bash
sudo ufw allow 80/tcp
sudo ufw allow 443/tcp
sudo ufw allow 22/tcp
sudo ufw enable
```

## 📊 Monitoring

```bash
# Service status
docker-compose ps

# Resource usage
docker stats

# Logs
docker-compose logs -f

# Database size
docker-compose exec db psql -U si3ln_user si3ln_db -c "\l+"

# Disk usage
docker system df
```

## 🐛 Troubleshooting

### API won't start
```bash
docker-compose logs api
docker-compose restart api
docker-compose exec api python manage.py check
```

### Database errors
```bash
docker-compose logs db
docker-compose restart db
docker-compose exec api python manage.py migrate
```

### Connection refused
```bash
# Check if services are running
docker-compose ps

# Check network
docker network ls
docker network inspect si3ln_si3ln_network
```

### Port conflicts
```bash
sudo lsof -i :8000
sudo lsof -i :5432
# Kill process or change ports in docker-compose.yml
```

## 📝 Tech Stack

### Backend
- **Django 4.2** - Web framework
- **Django Ninja 1.5** - REST API framework
- **PostgreSQL 15** - Database
- **Redis 7** - Caching
- **Gunicorn 21** - WSGI server
- **Nginx** - Reverse proxy
- **PyJWT 2.8** - JWT tokens

### Frontend (Game)
- **C++17** - Language
- **SFML 2.5** - Graphics & input
- **libcurl** - HTTP client
- **nlohmann-json** - JSON parsing
- **CMake 3.10+** - Build system

### DevOps
- **Docker** - Containerization
- **Docker Compose** - Orchestration
- **Git** - Version control

## 🤝 Contributing

1. Fork the repository
2. Create feature branch: `git checkout -b feature-name`
3. Commit changes: `git commit -am 'Add feature'`
4. Push to branch: `git push origin feature-name`
5. Submit pull request

## 📄 License

[Your License Here]

## 👥 Authors

[Your Name/Team]

## 🎯 Roadmap

- [ ] Add WebSocket support for real-time multiplayer
- [ ] Implement achievements system
- [ ] Add replay functionality
- [ ] Mobile client support
- [ ] Tournament bracket system
- [ ] Social features (friends, chat)
- [ ] Analytics dashboard

## 💡 Tips

- Use `./generate-secrets.sh` to generate production secrets
- Run `docker-compose logs -f` to debug issues
- Backup database regularly with pg_dump
- Monitor resource usage with `docker stats`
- Keep secrets out of git (use .gitignore)
- Test authentication flow after deployment

## 🔗 Links

- API Documentation: http://localhost:8000/api/docs
- Admin Panel: http://localhost:8000/admin
- Source Code: [Your Repository]
- Issues: [Your Issues Page]

---

**Built with ❤️ for gamers by gamers**
