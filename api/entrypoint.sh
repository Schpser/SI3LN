#!/bin/bash
set -e

echo "🐳 Starting SI3LN API in Docker..."

# Wait for database (if using PostgreSQL)
if [ -n "$DATABASE_URL" ]; then
    echo "⏳ Waiting for database..."
    sleep 2
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
    User.objects.create_superuser('admin', 'admin@example.com', 'admin123')
    print("✅ Superuser 'admin' created! (password: admin123)")
else:
    print("ℹ️  Superuser already exists")
EOF

# Collect static files
echo "📦 Collecting static files..."
python manage.py collectstatic --noinput || true

echo "🚀 Starting Gunicorn server on 0.0.0.0:8000..."
exec gunicorn si3ln_api.wsgi:application \
    --bind 0.0.0.0:8000 \
    --workers 4 \
    --timeout 120 \
    --access-logfile - \
    --error-logfile - \
    --log-level info
