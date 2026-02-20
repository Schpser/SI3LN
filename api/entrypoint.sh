#!/bin/bash
set -e

echo "🐳 Starting SI3LN API in Docker..."

# Wait for database (if using PostgreSQL)
if [ -n "$DATABASE_URL" ]; then
    echo "⏳ Waiting for database..."
    
    # Extract database host from DATABASE_URL
    DB_HOST=$(echo $DATABASE_URL | sed -E 's|.*@([^:/]+).*|\1|')
    DB_PORT=$(echo $DATABASE_URL | sed -E 's|.*:([0-9]+)/.*|\1|')
    
    echo "   Database: $DB_HOST:$DB_PORT"
    
    # Wait for PostgreSQL to be ready
    until pg_isready -h "$DB_HOST" -p "$DB_PORT" -U si3ln_user > /dev/null 2>&1; do
        echo "   > Database is unavailable - sleeping"
        sleep 1
    done
    
    echo "   ✅ Database is ready!"
fi

# Run migrations
echo "🔄 Running database migrations..."
python manage.py migrate --noinput

# Create superuser if not exists
echo "👤 Creating superuser if needed..."
python manage.py shell << EOF
from django.contrib.auth import get_user_model
User = get_user_model()
if not User.objects.filter(is_superuser=True).exists():
    User.objects.create_superuser('admin', 'admin@si3ln.local', 'admin123')
    print("✅ Superuser 'admin' created! (password: admin123)")
else:
    print("ℹ️  Superuser already exists")
EOF

# Collect static files
echo "📦 Collecting static files..."
python manage.py collectstatic --noinput || true

echo ""
echo "╔═══════════════════════════════════════════╗"
echo "║   🚀 SI3LN API Server Starting...        ║"
echo "╚═══════════════════════════════════════════╝"
echo ""
echo "📚 API Documentation: http://localhost:8000/api/docs"
echo "🔧 Admin Panel: http://localhost:8000/admin"
echo "   Username: admin"
echo "   Password: admin123"
echo ""

exec gunicorn si3ln_api.wsgi:application \
    --bind 0.0.0.0:8000 \
    --workers 4 \
    --timeout 120 \
    --access-logfile - \
    --error-logfile - \
    --log-level info
