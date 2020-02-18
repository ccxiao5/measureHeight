Page({
  data: {
    items: [],//用来显示输出
    startX: 0,
    startY: 0,
    isClick:0,//判断是否查询
    dbitem:[]//用来存储数据库数据
  },
  onLoad: function() {
    var that = this;
    wx.request({
      url: 'https://www.chenghuwei.cn//info.php',
      data:{
      },
      header:{
        'content-type':'application/json'
      },
      method:"POST",
      success:function(res){
        console.log("OnLoad:");
        console.log(res.data);
        that.setData({
          items: res.data,
          dbitem:res.data
        })
      }
    })
  },
  onPullDownRefresh:function(){
    var that = this;
    wx.request({
      url: 'https://www.chenghuwei.cn//info.php',
      data: {
      },
      header: {
        'content-type': 'application/json'
      },
      method: "POST",
      success: function (res) {
        console.log("Refresh:");
        console.log(res.data);
        that.setData({
          items: res.data,
          dbitem: res.data
        })
      }
    })
  },
  sortName() {
    var that=this;
    var sortName = [{}];
    var item = this.data.dbitem;
    var nameVector=[];
    var i = 0,index;
    for (index in item)
      if (nameVector.indexOf(item[index].Name)==-1)//如果不存在重复元素，就添加到nameVector
        nameVector[i++]=item[index].Name;
    i=0;  
    if (that.data.isClick)
    {
      that.setData({
          items: item,
          isClick:0
        })
    }
    else
    {
      var that=this;
      wx.showActionSheet({
        itemList: nameVector,
        success: function(res) {
          for ( index in item)
            if (item[index].Name == nameVector[res.tapIndex]) {//判断用户选中的是哪个用户
              sortName[i++] = item[index];
            }
          that.setData({
            items: sortName,
            isClick: 1
          })
        },
        fail: function(res) {
          console.log("查找失败");
        },
      })
    }
  },
  touchstart: function(e) {
    //开始触摸时 重置所有删除
    var index=e.currentTarget.dataset.index;
    var isTouch = this.data.items[index].isTouchMove;
    this.data.items.forEach(function(v, i) {
      if (v.isTouchMove) //只操作为true的
        v.isTouchMove = false;
    })
    if (!isTouch)//改变点击后元素isTouchMove
      this.data.items[index].isTouchMove = true;
    this.setData({
      startX: e.changedTouches[0].clientX,
      startY: e.changedTouches[0].clientY,
      items:this.data.items
    })
  },
  touchmove: function(e) {
    var that = this,
      index = e.currentTarget.dataset.index, //当前索引
      startX = that.data.startX, //开始X坐标
      startY = that.data.startY, //开始Y坐标
      touchMoveX = e.changedTouches[0].clientX, //滑动变化坐标
      touchMoveY = e.changedTouches[0].clientY, //滑动变化坐标
      //获取滑动角度
      angle = that.angle({
        X: startX,
        Y: startY
      }, {
        X: touchMoveX,
        Y: touchMoveY
      });
    that.data.items.forEach(function(v, i) {
      v.isTouchMove = false
      //滑动超过30度角 return
      if (Math.abs(angle) > 30) return;
      if (i == index) {
        if (touchMoveX > startX) //右滑
          v.isTouchMove = false
        else //左滑
          v.isTouchMove = true
      }
    })
    //更新数据
    that.setData({
      items: that.data.items
    })
  },
  /**

* 计算滑动角度
* @param {Object} start 起点坐标
* @param {Object} end 终点坐标

*/
  angle: function(start, end) {
    var _X = end.X - start.X,
      _Y = end.Y - start.Y
    //返回角度 /Math.atan()返回数字的反正切值
    return 360 * Math.atan(_Y / _X) / (2 * Math.PI);
  },
  //删除事件
  del: function(e) {
    var that=this;
    //删除数据库中数据
    var array = this.data.items[e.currentTarget.dataset.index];
    wx.request({
      url: 'https://www.chenghuwei.cn//del.php',
      header: {
        'content-type': 'application/json'
      },
      method: "GET",
      data: {
        measureDate:array["Date"],
        measureName:array["Name"],
        height:array["height"]
      },
      success: function (res) {
        console.log("DEL:");
        console.log(res.data);
        that.setData({
          items: res.data,
          dbitem: res.data
        })
      }
    })
    this.data.items.splice(e.currentTarget.dataset.index, 1)
    this.setData({
      items: this.data.items,
    })
 }
})