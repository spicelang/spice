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
define internal void @_Z7takeRefRK5Thingb(ptr dead_on_unwind noalias writable sret(%struct.Thing) align 1 %0, ptr noundef %1, i1 noundef zeroext %2) #0 {
  %ref = alloca ptr, align 8
  %cond = alloca i1, align 1
  %4 = alloca %struct.Thing, align 8
  %5 = alloca %struct.Thing, align 8
  %res = alloca %struct.Thing, align 8
  store ptr %1, ptr %ref, align 8
  store i1 %2, ptr %cond, align 1
  %6 = load i1, ptr %cond, align 1
  br i1 %6, label %cond.true.L13C23, label %cond.false.L13C23

cond.true.L13C23:                                 ; preds = %3
  %7 = load ptr, ptr %ref, align 8
  call void @_ZN5Thing4ctorERK5Thing(ptr noundef nonnull align 1 %4, ptr %7)
  br label %cond.exit.L13C23

cond.false.L13C23:                                ; preds = %3
  call void @_ZN5Thing4ctorEv(ptr noundef nonnull align 1 %5)
  br label %cond.exit.L13C23

cond.exit.L13C23:                                 ; preds = %cond.false.L13C23, %cond.true.L13C23
  %cond.result = phi ptr [ %4, %cond.true.L13C23 ], [ %5, %cond.false.L13C23 ]
  call void @llvm.memcpy.p0.p0.i64(ptr %res, ptr %cond.result, i64 0, i1 false)
  %8 = load %struct.Thing, ptr %res, align 1
  store %struct.Thing %8, ptr %0, align 1
  ret void
}

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(argmem: readwrite)
declare void @llvm.memcpy.p0.p0.i64(ptr noalias writeonly captures(none), ptr noalias readonly captures(none), i64, i1 immarg) #2

; Function Attrs: noinline nounwind optnone uwtable
define internal void @_Z7takeVal5Thingb(ptr dead_on_unwind noalias writable sret(%struct.Thing) align 1 %0, %struct.Thing noundef %1, i1 noundef zeroext %2) #0 {
  %val = alloca %struct.Thing, align 8
  %cond = alloca i1, align 1
  %4 = alloca %struct.Thing, align 8
  %5 = alloca %struct.Thing, align 8
  %res = alloca %struct.Thing, align 8
  store %struct.Thing %1, ptr %val, align 1
  store i1 %2, ptr %cond, align 1
  %6 = load i1, ptr %cond, align 1
  br i1 %6, label %cond.true.L19C23, label %cond.false.L19C23

cond.true.L19C23:                                 ; preds = %3
  call void @_ZN5Thing4ctorERK5Thing(ptr noundef nonnull align 1 %4, ptr %val)
  br label %cond.exit.L19C23

cond.false.L19C23:                                ; preds = %3
  call void @_ZN5Thing4ctorEv(ptr noundef nonnull align 1 %5)
  br label %cond.exit.L19C23

cond.exit.L19C23:                                 ; preds = %cond.false.L19C23, %cond.true.L19C23
  %cond.result = phi ptr [ %4, %cond.true.L19C23 ], [ %5, %cond.false.L19C23 ]
  call void @llvm.memcpy.p0.p0.i64(ptr %res, ptr %cond.result, i64 0, i1 false)
  %7 = load %struct.Thing, ptr %res, align 1
  store %struct.Thing %7, ptr %0, align 1
  ret void
}

; Function Attrs: mustprogress noinline norecurse nounwind optnone uwtable
define dso_local noundef i32 @main() #3 {
  %t = alloca %struct.Thing, align 8
  %viaRef = alloca %struct.Thing, align 8
  %viaRefTemp = alloca %struct.Thing, align 8
  %arg.copy = alloca %struct.Thing, align 8
  %viaVal = alloca %struct.Thing, align 8
  %arg.copy1 = alloca %struct.Thing, align 8
  %viaValTemp = alloca %struct.Thing, align 8
  call void @_ZN5Thing4ctorEv(ptr noundef nonnull align 1 %t)
  call void @_Z7takeRefRK5Thingb(ptr dead_on_unwind writable sret(%struct.Thing) align 1 %viaRef, ptr noundef %t, i1 noundef zeroext true)
  call void @_ZN5Thing4dtorEv(ptr noundef nonnull align 1 %viaRef)
  call void @_Z7takeRefRK5Thingb(ptr dead_on_unwind writable sret(%struct.Thing) align 1 %viaRefTemp, ptr noundef %t, i1 noundef zeroext false)
  call void @_ZN5Thing4dtorEv(ptr noundef nonnull align 1 %viaRefTemp)
  call void @_ZN5Thing4ctorERK5Thing(ptr noundef nonnull align 1 %arg.copy, ptr %t)
  %1 = load %struct.Thing, ptr %arg.copy, align 1
  call void @_Z7takeVal5Thingb(ptr dead_on_unwind writable sret(%struct.Thing) align 1 %viaVal, %struct.Thing noundef %1, i1 noundef zeroext true)
  call void @_ZN5Thing4dtorEv(ptr noundef nonnull align 1 %arg.copy)
  call void @_ZN5Thing4dtorEv(ptr noundef nonnull align 1 %viaVal)
  call void @_ZN5Thing4ctorERK5Thing(ptr noundef nonnull align 1 %arg.copy1, ptr %t)
  %2 = load %struct.Thing, ptr %arg.copy1, align 1
  call void @_Z7takeVal5Thingb(ptr dead_on_unwind writable sret(%struct.Thing) align 1 %viaValTemp, %struct.Thing noundef %2, i1 noundef zeroext false)
  call void @_ZN5Thing4dtorEv(ptr noundef nonnull align 1 %arg.copy1)
  call void @_ZN5Thing4dtorEv(ptr noundef nonnull align 1 %viaValTemp)
  call void @_ZN5Thing4dtorEv(ptr noundef nonnull align 1 %t)
  ret i32 0
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
!3 = !{i32 7, !"frame-pointer", i32 0}
!4 = !{!"spice version dev [host] (https://github.com/spicelang/spice)"}
