class MySQLRouter:
    def db_for_read(self, model, **hints):
        return 'mysql'

    def db_for_write(self, model, **hints):
        return 'mysql'

    def allow_relation(self, obj1, obj2, **hints):
        return True

    def allow_migrate(self, db, app_label, model_name=None, **hints):
        return db == 'mysql'
