; ModuleID = 'source.spice'
source_filename = "source.spice"

%struct.Inner = type { ptr, ptr }
%struct.Outer = type { %struct.Middle }
%struct.Middle = type { %struct.Inner }

@anon.string.0 = private unnamed_addr constant [12 x i8] c"Hello World\00", align 1
@printf.str.0 = private unnamed_addr constant [19 x i8] c"Inner dtor called\0A\00", align 1

declare ptr @malloc(i64 noundef)

declare void @free(ptr noundef)

; Function Attrs: norecurse
define void @_ZN5Inner4ctorERK5Inner(ptr noundef nonnull align 8 dereferenceable(16) %0, ptr %1) #0 {
  %this = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  %3 = load ptr, ptr %this, align 8
  call void @llvm.memcpy.p0.p0.i64(ptr %3, ptr %1, i64 8, i1 false)
  %4 = getelementptr inbounds nuw %struct.Inner, ptr %1, i32 0, i32 1
  %5 = getelementptr inbounds nuw %struct.Inner, ptr %3, i32 0, i32 1
  %6 = load ptr, ptr %4, align 8
  %7 = icmp ne ptr %6, null
  br i1 %7, label %nullptrcheck.then, label %nullptrcheck.exit

nullptrcheck.then:                                ; preds = %2
  %8 = call ptr @_Z12sAllocUnsafem(i64 1)
  store ptr %8, ptr %5, align 8
  call void @llvm.memcpy.p0.p0.i64(ptr %8, ptr %6, i64 1, i1 false)
  br label %nullptrcheck.exit

nullptrcheck.exit:                                ; preds = %nullptrcheck.then, %2
  ret void
}

; Function Attrs: nocallback nofree nounwind willreturn memory(argmem: readwrite)
declare void @llvm.memcpy.p0.p0.i64(ptr noalias nocapture writeonly, ptr noalias nocapture readonly, i64, i1 immarg) #1

declare ptr @_Z12sAllocUnsafem(i64)

define private void @_ZN5Inner4ctorEv(ptr noundef nonnull align 8 dereferenceable(16) %0) {
  %this = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  %2 = load ptr, ptr %this, align 8
  store ptr @anon.string.0, ptr %2, align 8
  %3 = getelementptr inbounds nuw %struct.Inner, ptr %2, i32 0, i32 1
  store ptr null, ptr %3, align 8
  %4 = load ptr, ptr %this, align 8
  %data.addr = getelementptr inbounds %struct.Inner, ptr %4, i64 0, i32 1
  %5 = call ptr @malloc(i64 10)
  store ptr %5, ptr %data.addr, align 8
  ret void
}

define private void @_ZN5Inner4dtorEv(ptr noundef nonnull align 8 dereferenceable(16) %0) {
  %this = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  %2 = load ptr, ptr %this, align 8
  %data.addr = getelementptr inbounds %struct.Inner, ptr %2, i64 0, i32 1
  %3 = load ptr, ptr %data.addr, align 8
  call void @free(ptr %3)
  %4 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0)
  ret void
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) #2

; Function Attrs: norecurse
define void @_ZN6Middle4ctorEv(ptr noundef nonnull align 8 dereferenceable(16) %0) #0 {
  %this = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  %2 = load ptr, ptr %this, align 8
  call void @_ZN5Inner4ctorEv(ptr %2)
  ret void
}

; Function Attrs: norecurse
define void @_ZN6Middle4ctorERK6Middle(ptr noundef nonnull align 8 dereferenceable(16) %0, ptr %1) #0 {
  %this = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  %3 = load ptr, ptr %this, align 8
  call void @_ZN5Inner4ctorERK5Inner(ptr %3, ptr %1)
  ret void
}

; Function Attrs: norecurse
define void @_ZN6Middle4dtorEv(ptr noundef nonnull align 8 dereferenceable(16) %0) #0 {
  %this = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  %2 = load ptr, ptr %this, align 8
  call void @_ZN5Inner4dtorEv(ptr %2)
  ret void
}

; Function Attrs: norecurse
define void @_ZN5Outer4ctorEv(ptr noundef nonnull align 8 dereferenceable(16) %0) #0 {
  %this = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  %2 = load ptr, ptr %this, align 8
  call void @_ZN6Middle4ctorEv(ptr %2)
  ret void
}

; Function Attrs: norecurse
define void @_ZN5Outer4dtorEv(ptr noundef nonnull align 8 dereferenceable(16) %0) #0 {
  %this = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  %2 = load ptr, ptr %this, align 8
  call void @_ZN6Middle4dtorEv(ptr %2)
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #3 {
  %result = alloca i32, align 4
  %outer = alloca %struct.Outer, align 8
  store i32 0, ptr %result, align 4
  call void @_ZN5Outer4ctorEv(ptr %outer)
  call void @_ZN5Outer4dtorEv(ptr %outer)
  %1 = load i32, ptr %result, align 4
  ret i32 %1
}

attributes #0 = { norecurse }
attributes #1 = { nocallback nofree nounwind willreturn memory(argmem: readwrite) }
attributes #2 = { nofree nounwind }
attributes #3 = { noinline nounwind optnone uwtable }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 2}
!4 = !{!"spice version dev (https://github.com/spicelang/spice)"}
