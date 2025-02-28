; ModuleID = 'source.spice'
source_filename = "source.spice"

%struct.Outer = type { %struct.Middle }
%struct.Middle = type { %struct.Inner }
%struct.Inner = type { ptr }

@anon.string.0 = private unnamed_addr constant [12 x i8] c"Hello World\00", align 1
@printf.str.0 = private unnamed_addr constant [13 x i8] c"Message: %s\0A\00", align 1

; Function Attrs: norecurse
define void @_ZN5Inner4ctorEv(ptr noundef nonnull align 8 dereferenceable(8) %0) #0 {
  %this = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  %2 = load ptr, ptr %this, align 8
  store ptr @anon.string.0, ptr %2, align 8
  ret void
}

; Function Attrs: norecurse
define void @_ZN6Middle4ctorEv(ptr noundef nonnull align 8 dereferenceable(8) %0) #0 {
  %this = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  %2 = load ptr, ptr %this, align 8
  call void @_ZN5Inner4ctorEv(ptr %2)
  ret void
}

; Function Attrs: norecurse
define void @_ZN5Outer4ctorEv(ptr noundef nonnull align 8 dereferenceable(8) %0) #0 {
  %this = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  %2 = load ptr, ptr %this, align 8
  call void @_ZN6Middle4ctorEv(ptr %2)
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #1 {
  %result = alloca i32, align 4
  %outer = alloca %struct.Outer, align 8
  store i32 0, ptr %result, align 4
  call void @_ZN5Outer4ctorEv(ptr %outer)
  %middle.addr = getelementptr inbounds %struct.Outer, ptr %outer, i64 0, i32 0
  %inner.addr = getelementptr inbounds %struct.Middle, ptr %middle.addr, i64 0, i32 0
  %message.addr = getelementptr inbounds %struct.Inner, ptr %inner.addr, i64 0, i32 0
  %1 = load ptr, ptr %message.addr, align 8
  %2 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0, ptr %1)
  %3 = load i32, ptr %result, align 4
  ret i32 %3
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) #2

attributes #0 = { norecurse }
attributes #1 = { noinline nounwind optnone uwtable }
attributes #2 = { nofree nounwind }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 2}
!4 = !{!"spice version dev (https://github.com/spicelang/spice)"}
