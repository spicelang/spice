; ModuleID = 'source.spice'
source_filename = "source.spice"

%struct.Thing = type {}

@printf.str.0 = private unnamed_addr constant [7 x i8] c"Ctor!\0A\00", align 4
@printf.str.1 = private unnamed_addr constant [12 x i8] c"Copy ctor!\0A\00", align 4
@printf.str.2 = private unnamed_addr constant [7 x i8] c"Dtor!\0A\00", align 4

; Function Attrs: noinline nounwind optnone uwtable
define internal void @_ZN5Thing4ctorEv(ptr noundef nonnull align 1 %0) #0 {
  %this = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  %2 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.0)
  ret void
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr noundef readonly captures(none), ...) local_unnamed_addr #1

; Function Attrs: noinline nounwind optnone uwtable
define internal void @_ZN5Thing4ctorERK5Thing(ptr noundef nonnull align 1 %0, ptr noundef %1) #0 {
  %this = alloca ptr, align 8
  %_ = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  store ptr %1, ptr %_, align 8
  %3 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.1)
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define internal void @_ZN5Thing4dtorEv(ptr noundef nonnull align 1 %0) #0 {
  %this = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  %2 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.2)
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define internal noundef %struct.Thing @_Z7takeRefRK5Thingb(ptr noundef %0, i1 noundef zeroext %1) #0 {
  %result = alloca %struct.Thing, align 8
  %ref = alloca ptr, align 8
  %cond = alloca i1, align 1
  %3 = alloca %struct.Thing, align 8
  %4 = alloca %struct.Thing, align 8
  %res = alloca %struct.Thing, align 8
  store ptr %0, ptr %ref, align 8
  store i1 %1, ptr %cond, align 1
  %5 = load i1, ptr %cond, align 1
  br i1 %5, label %cond.true.L13C23, label %cond.false.L13C23

cond.true.L13C23:                                 ; preds = %2
  %6 = load ptr, ptr %ref, align 8
  call void @_ZN5Thing4ctorERK5Thing(ptr noundef nonnull align 1 %3, ptr %6)
  br label %cond.exit.L13C23

cond.false.L13C23:                                ; preds = %2
  call void @_ZN5Thing4ctorEv(ptr noundef nonnull align 1 %4)
  br label %cond.exit.L13C23

cond.exit.L13C23:                                 ; preds = %cond.false.L13C23, %cond.true.L13C23
  %cond.result = phi ptr [ %3, %cond.true.L13C23 ], [ %4, %cond.false.L13C23 ]
  call void @llvm.memcpy.p0.p0.i64(ptr %res, ptr %cond.result, i64 0, i1 false)
  %7 = load %struct.Thing, ptr %res, align 1
  ret %struct.Thing %7
}

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(argmem: readwrite)
declare void @llvm.memcpy.p0.p0.i64(ptr noalias writeonly captures(none), ptr noalias readonly captures(none), i64, i1 immarg) #2

; Function Attrs: noinline nounwind optnone uwtable
define internal noundef %struct.Thing @_Z7takeVal5Thingb(%struct.Thing noundef %0, i1 noundef zeroext %1) #0 {
  %result = alloca %struct.Thing, align 8
  %val = alloca %struct.Thing, align 8
  %cond = alloca i1, align 1
  %3 = alloca %struct.Thing, align 8
  %4 = alloca %struct.Thing, align 8
  %res = alloca %struct.Thing, align 8
  store %struct.Thing %0, ptr %val, align 1
  store i1 %1, ptr %cond, align 1
  %5 = load i1, ptr %cond, align 1
  br i1 %5, label %cond.true.L19C23, label %cond.false.L19C23

cond.true.L19C23:                                 ; preds = %2
  call void @_ZN5Thing4ctorERK5Thing(ptr noundef nonnull align 1 %3, ptr %val)
  br label %cond.exit.L19C23

cond.false.L19C23:                                ; preds = %2
  call void @_ZN5Thing4ctorEv(ptr noundef nonnull align 1 %4)
  br label %cond.exit.L19C23

cond.exit.L19C23:                                 ; preds = %cond.false.L19C23, %cond.true.L19C23
  %cond.result = phi ptr [ %3, %cond.true.L19C23 ], [ %4, %cond.false.L19C23 ]
  call void @llvm.memcpy.p0.p0.i64(ptr %res, ptr %cond.result, i64 0, i1 false)
  %6 = load %struct.Thing, ptr %res, align 1
  ret %struct.Thing %6
}

; Function Attrs: mustprogress noinline norecurse nounwind optnone uwtable
define dso_local noundef i32 @main() #3 {
  %result = alloca i32, align 4
  %t = alloca %struct.Thing, align 8
  %viaRef = alloca %struct.Thing, align 8
  %viaRefTemp = alloca %struct.Thing, align 8
  %arg.copy = alloca %struct.Thing, align 8
  %viaVal = alloca %struct.Thing, align 8
  %arg.copy1 = alloca %struct.Thing, align 8
  %viaValTemp = alloca %struct.Thing, align 8
  store i32 0, ptr %result, align 4
  call void @_ZN5Thing4ctorEv(ptr noundef nonnull align 1 %t)
  %1 = call noundef %struct.Thing @_Z7takeRefRK5Thingb(ptr noundef %t, i1 noundef zeroext true)
  store %struct.Thing %1, ptr %viaRef, align 1
  call void @_ZN5Thing4dtorEv(ptr noundef nonnull align 1 %viaRef)
  %2 = call noundef %struct.Thing @_Z7takeRefRK5Thingb(ptr noundef %t, i1 noundef zeroext false)
  store %struct.Thing %2, ptr %viaRefTemp, align 1
  call void @_ZN5Thing4dtorEv(ptr noundef nonnull align 1 %viaRefTemp)
  call void @_ZN5Thing4ctorERK5Thing(ptr noundef nonnull align 1 %arg.copy, ptr %t)
  %3 = load %struct.Thing, ptr %arg.copy, align 1
  %4 = call noundef %struct.Thing @_Z7takeVal5Thingb(%struct.Thing noundef %3, i1 noundef zeroext true)
  store %struct.Thing %4, ptr %viaVal, align 1
  call void @_ZN5Thing4dtorEv(ptr noundef nonnull align 1 %arg.copy)
  call void @_ZN5Thing4dtorEv(ptr noundef nonnull align 1 %viaVal)
  call void @_ZN5Thing4ctorERK5Thing(ptr noundef nonnull align 1 %arg.copy1, ptr %t)
  %5 = load %struct.Thing, ptr %arg.copy1, align 1
  %6 = call noundef %struct.Thing @_Z7takeVal5Thingb(%struct.Thing noundef %5, i1 noundef zeroext false)
  store %struct.Thing %6, ptr %viaValTemp, align 1
  call void @_ZN5Thing4dtorEv(ptr noundef nonnull align 1 %arg.copy1)
  call void @_ZN5Thing4dtorEv(ptr noundef nonnull align 1 %viaValTemp)
  call void @_ZN5Thing4dtorEv(ptr noundef nonnull align 1 %t)
  %7 = load i32, ptr %result, align 4
  ret i32 %7
}

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }
attributes #2 = { nocallback nofree nosync nounwind willreturn memory(argmem: readwrite) }
attributes #3 = { mustprogress noinline norecurse nounwind optnone uwtable }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 2}
!4 = !{!"spice version dev (https://github.com/spicelang/spice)"}
