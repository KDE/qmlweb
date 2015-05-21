(function() {
    QW_INHERIT(Rect, QWObject);

    function Rect(parent)
    {
        QWObject.call(this, parent);
        var self = this;

        this.width = new QWProperty({ type: QWInt, initialValue: 20 });
        this.height = new QWProperty({
            type: QWInt,
            initialValue: undefined,
            get: function() {
                return this.value === undefined ? self.width.value : this.value; // don't ask me, why we want a square, by default. It's a test!
            },
            set: function(newValue) {
                this.value = newValue === undefined? newValue = undefined : newValue + 1; // This doesn't make any sense, but it's only a test after all, isn't it?
            }
        });
    }

    return {
        Rect: Rect
    }
})();
