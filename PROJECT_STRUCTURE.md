# 📁 SI3LN Project Structure

## Current Organization

```
SI3LN/
├── 📦 Backend API (Django)
│   ├── api/
│   │   ├── game/                    # Game app
│   │   │   ├── models.py           # Database models
│   │   │   ├── api.py              # Game endpoints
│   │   │   ├── auth_api.py         # Authentication endpoints
│   │   │   ├── jwt_auth.py         # JWT handler
│   │   │   └── auth_decorators.py  # Auth middleware
│   │   ├── si3ln_api/              # Django project
│   │   │   ├── settings.py         # Configuration
│   │   │   ├── urls.py             # URL routing
│   │   │   └── wsgi.py             # WSGI config
│   │   ├── manage.py               # Django CLI
│   │   ├── requirements.txt        # Python dependencies
│   │   ├── Dockerfile              # API container
│   │   └── test_api.py            # API tests
│   │
├── 🎮 Game Engine (C++)
│   ├── game_engine_C++/
│   │   ├── src/                    # Source files
│   │   │   ├── main.cpp
│   │   │   ├── Game.cpp
│   │   │   ├── Player.cpp
│   │   │   ├── Enemy.cpp
│   │   │   └── APIClient.cpp      # HTTP client
│   │   ├── include/                # Headers
│   │   │   ├── Game.h
│   │   │   ├── Player.h
│   │   │   ├── Enemy.h
│   │   │   └── APIClient.h
│   │   ├── CMakeLists.txt         # Build config
│   │   ├── build.sh               # Build script
│   │   └── Dockerfile             # Game container
│   │
├── 🐳 Infrastructure
│   ├── docker-compose.yml          # Service orchestration
│   ├── infrastructure/
│   │   └── nginx/
│   │       └── nginx.conf         # Reverse proxy
│   │
├── 🚀 Deployment Scripts
│   ├── deploy.sh                   # Main deployment
│   ├── generate-secrets.sh         # Security keys
│   ├── health-check.sh            # Service monitor
│   │
├── 📚 Documentation
│   ├── README.md                   # Project overview
│   ├── QUICKSTART.md              # Quick start guide
│   ├── DOCKER_DEPLOYMENT.md       # Docker guide
│   ├── DOCKER_COMPLETE_GUIDE.md   # Comprehensive guide
│   └── PROJECT_STRUCTURE.md       # This file
│   │
└── ⚙️  Configuration
    ├── .env.production.example    # Production template
    ├── .gitignore                 # Git ignore rules
    └── DEPLOYMENT_STATUS.txt      # Status summary
```

## Recommended Structure (Best Practices)

For better organization, here's the ideal structure:

```
SI3LN/
│
├── 📁 backend/                     # Renamed from 'api'
│   ├── apps/
│   │   ├── game/                  # Game logic
│   │   ├── auth/                  # Authentication (separate)
│   │   └── leaderboard/           # Leaderboard (could be separate)
│   ├── core/                      # Project settings
│   ├── tests/                     # All tests
│   ├── docker/                    # Docker files
│   │   ├── Dockerfile
│   │   └── entrypoint.sh
│   ├── requirements/              # Organized dependencies
│   │   ├── base.txt
│   │   ├── development.txt
│   │   └── production.txt
│   └── manage.py
│
├── 📁 game/                       # Renamed from 'game_engine_C++'
│   ├── src/
│   ├── include/
│   ├── tests/                     # C++ tests
│   ├── assets/                    # Game resources
│   │   ├── sprites/
│   │   ├── sounds/
│   │   └── fonts/
│   ├── docker/
│   │   └── Dockerfile
│   └── CMakeLists.txt
│
├── 📁 infrastructure/             # DevOps & deployment
│   ├── docker/
│   │   ├── docker-compose.yml
│   │   ├── docker-compose.prod.yml
│   │   └── docker-compose.dev.yml
│   ├── nginx/
│   │   ├── nginx.conf
│   │   └── ssl/                   # SSL certificates
│   ├── kubernetes/                # K8s configs (future)
│   └── terraform/                 # IaC (future)
│
├── 📁 scripts/                    # Utility scripts
│   ├── deploy.sh
│   ├── generate-secrets.sh
│   ├── health-check.sh
│   ├── backup-db.sh
│   └── test-all.sh
│
├── 📁 docs/                       # All documentation
│   ├── README.md
│   ├── QUICKSTART.md
│   ├── API.md
│   ├── DEPLOYMENT.md
│   ├── ARCHITECTURE.md
│   └── CONTRIBUTING.md
│
└── 📁 config/                     # Configuration files
    ├── .env.example
    ├── .env.development
    ├── .env.production.example
    └── .gitignore
```

## Why This Structure?

### Benefits:
1. **Clear Separation** - Backend, game, infrastructure separated
2. **Scalability** - Easy to add new components
3. **Team Friendly** - Different teams can work independently
4. **CI/CD Ready** - Organized for automation
5. **Docker Optimized** - Each component can be built independently
6. **Documentation** - All docs in one place

### Current vs Recommended:

| Current | Recommended | Why |
|---------|-------------|-----|
| `api/` | `backend/` | More descriptive |
| `game_engine_C++/` | `game/` | Cleaner name |
| Root level scripts | `scripts/` | Organized |
| Mixed docs | `docs/` | Centralized |
| Single .env | `config/` | Environment-specific |

## File Organization Principles

### 1. Backend (Django)
```
backend/
├── apps/           # Each feature as separate app
├── core/           # Project-wide settings
├── tests/          # Centralized tests
└── requirements/   # Split by environment
```

### 2. Game (C++)
```
game/
├── src/            # Implementation
├── include/        # Public headers
├── tests/          # Unit tests
└── assets/         # Resources
```

### 3. Infrastructure
```
infrastructure/
├── docker/         # All docker configs
├── nginx/          # Web server
└── scripts/        # Deployment automation
```

## Docker Compose Structure

### Current (Single File)
```yaml
docker-compose.yml  # Everything in one
```

### Recommended (Multiple Files)
```yaml
docker-compose.yml           # Base config
docker-compose.dev.yml       # Development overrides
docker-compose.prod.yml      # Production overrides
docker-compose.test.yml      # Testing environment
```

Usage:
```bash
# Development
docker-compose -f docker-compose.yml -f docker-compose.dev.yml up

# Production
docker-compose -f docker-compose.yml -f docker-compose.prod.yml up
```

## Environment Configuration

### Current
```
.env                          # Single file
.env.production.example
```

### Recommended
```
config/
├── .env.example             # Template with all vars
├── .env.development         # Dev-specific
├── .env.test               # Test-specific
├── .env.production.example  # Prod template
└── .env.local              # Local overrides (gitignored)
```

## Testing Structure

### Backend Tests
```
backend/tests/
├── unit/                    # Unit tests
├── integration/             # Integration tests
├── e2e/                    # End-to-end tests
└── fixtures/               # Test data
```

### Game Tests
```
game/tests/
├── unit/                    # Unit tests
├── integration/             # Component tests
└── mocks/                  # Mock objects
```

## Scripts Organization

```
scripts/
├── deploy/
│   ├── deploy-dev.sh
│   ├── deploy-prod.sh
│   └── rollback.sh
├── database/
│   ├── backup.sh
│   ├── restore.sh
│   └── migrate.sh
├── monitoring/
│   ├── health-check.sh
│   └── logs.sh
└── setup/
    ├── install-deps.sh
    └── generate-secrets.sh
```

## Next Steps to Restructure

If you want to apply the recommended structure, we can:

1. **Gradual Migration** - Move files incrementally
2. **Keep Current** - Current structure works fine
3. **Hybrid Approach** - Apply some improvements

Current structure is **functional** for your needs. Restructuring is optional and mainly beneficial for:
- Large teams
- Multiple services
- Complex deployments
- Long-term maintenance

## Quick Reference

### Current Important Paths
- Backend API: `/home/schpser/SI3LN/api/`
- Game Engine: `/home/schpser/SI3LN/game_engine_C++/`
- Docker Config: `/home/schpser/SI3LN/docker-compose.yml`
- Deploy Script: `/home/schpser/SI3LN/deploy.sh`

### Key Commands
```bash
# Deploy
cd /home/schpser/SI3LN && ./deploy.sh dev

# Check status
docker-compose ps

# View logs
docker-compose logs -f

# Health check
./health-check.sh
```

---

**Your current structure is good for development. Restructuring is optional!**
