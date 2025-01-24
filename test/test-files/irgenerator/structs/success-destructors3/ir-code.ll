; ModuleID = 'source.spice'
source_filename = "source.spice"

%struct.String = type { ptr, i64, i64 }

@anon.string.0 = private unnamed_addr constant [8 x i8] c"default\00", align 1
@printf.str.0 = private unnamed_addr constant [3 x i8] c"%s\00", align 1
@printf.str.1 = private unnamed_addr constant [3 x i8] c"%s\00", align 1
@anon.string.1 = private unnamed_addr constant [5 x i8] c"test\00", align 1

define private void @_Z4testv() {
  %1 = alloca %struct.String, align 8
  %t = alloca ptr, align 8
  call void @_ZN6String4ctorEPKc(ptr noundef nonnull align 8 dereferenceable(24) %1, ptr @anon.string.0)
  store ptr %1, ptr %t, align 8
  %2 = load ptr, ptr %t, align 8
  %3 = load ptr, ptr %2, align 8
  %4 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0, ptr %3)
  call void @_ZN6String4dtorEv(ptr %1)
  ret void
}

declare void @_ZN6String4ctorEPKc(ptr, ptr)

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) #0

declare void @_ZN6String4dtorEv(ptr)

define private void @_Z4testRK6String(ptr %0) {
  %t = alloca ptr, align 8
  store ptr %0, ptr %t, align 8
  %2 = load ptr, ptr %t, align 8
  %3 = load ptr, ptr %2, align 8
  %4 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.1, ptr %3)
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #1 {
  %result = alloca i32, align 4
  %1 = alloca %struct.String, align 8
  store i32 0, ptr %result, align 4
  call void @_Z4testv()
  call void @_ZN6String4ctorEPKc(ptr noundef nonnull align 8 dereferenceable(24) %1, ptr @anon.string.1)
  call void @_Z4testRK6String(ptr %1)
  call void @_ZN6String4dtorEv(ptr %1)
  %2 = load i32, ptr %result, align 4
  ret i32 %2
}

attributes #0 = { nofree nounwind }
attributes #1 = { noinline nounwind optnone uwtable }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 2}
!4 = !{!"spice version dev (https://github.com/spicelang/spice)"}
