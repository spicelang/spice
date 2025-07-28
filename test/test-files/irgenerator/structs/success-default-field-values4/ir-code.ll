; ModuleID = 'source.spice'
source_filename = "source.spice"

%struct.Test = type { i32, ptr }

@0 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@printf.str.0 = private unnamed_addr constant [9 x i8] c"Int: %d\0A\00", align 1
@printf.str.1 = private unnamed_addr constant [12 x i8] c"String: %s\0A\00", align 1

define private void @_ZN4Test4ctorEv(ptr noundef nonnull align 8 dereferenceable(16) %0) {
  %this = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  %2 = load ptr, ptr %this, align 8
  store i32 12, ptr %2, align 4
  %3 = getelementptr inbounds nuw %struct.Test, ptr %2, i32 0, i32 1
  store ptr @0, ptr %3, align 8
  %4 = load ptr, ptr %this, align 8
  %i.addr = getelementptr inbounds %struct.Test, ptr %4, i64 0, i32 0
  store i32 14, ptr %i.addr, align 4
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %result = alloca i32, align 4
  %t = alloca %struct.Test, align 8
  store i32 0, ptr %result, align 4
  call void @_ZN4Test4ctorEv(ptr noundef nonnull align 8 dereferenceable(16) %t)
  %i.addr = getelementptr inbounds %struct.Test, ptr %t, i64 0, i32 0
  %1 = load i32, ptr %i.addr, align 4
  %2 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i32 %1)
  %s.addr = getelementptr inbounds %struct.Test, ptr %t, i64 0, i32 1
  %3 = load ptr, ptr %s.addr, align 8
  %4 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.1, ptr %3)
  %5 = load i32, ptr %result, align 4
  ret i32 %5
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr noundef readonly captures(none), ...) #1

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 2}
!4 = !{!"spice version dev (https://github.com/spicelang/spice)"}
