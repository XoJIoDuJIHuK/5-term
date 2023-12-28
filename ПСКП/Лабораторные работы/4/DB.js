class DB {
    constructor(initialData) {
        this.data = initialData
        this.idSet = new Set()
        for (let row of initialData) {
            if (!this.idSet.has(row.id)) this.idSet.add(row.id)
        }
    }

    async select() {
        return new Promise((resolve) => {
            resolve(JSON.stringify(this.data.filter(element => element)))
        })
    }

    async insert(row) {
        console.log(row)
        return new Promise((resolve) => {
            if (!this.idSet.has(row.id))
            {
                this.data[row.id] = row
                this.idSet.add(row.id)
            }
            resolve(JSON.stringify(this.data.filter(element => element)))
        })
    }

    async update(row) {
        let index = row.id
        return new Promise((resolve) => {
            if (index >= 0 && index < this.data.length) {
                this.data[index] = row
            }
            resolve(row)
        })
    }

    async delete(index) {
        return new Promise((resolve, reject) => {
            let ret
            if (this.data[index]) {
                ret = this.data[index]
                this.data[index] = undefined
                this.idSet.delete(index)
                resolve(JSON.stringify(ret))
            }
            else reject("No such row")
        })
    }
}

module.exports = DB