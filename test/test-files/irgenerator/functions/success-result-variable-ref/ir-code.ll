; ModuleID = 'source.spice'
source_filename = "source.spice"

%struct.Test = type { i32 }

@printf.str.0 = private unnamed_addr constant [16 x i8] c"Field value: %d\00", align 4

define private noundef ptr @_ZN4Test8getFieldEv(ptr noundef nonnull align 4 dereferenceable(4) %0) {
  %result = alloca ptr, align 8
  %this = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  %2 = load ptr, ptr %this, align 8
  %field.addr = getelementptr inbounds %struct.Test, ptr %2, i64 0, i32 0
  store ptr %field.addr, ptr %result, align 8
  %3 = load ptr, ptr %result, align 8
  ret ptr %3
}

; Function Attrs: mustprogress noinline norecurse nounwind optnone uwtable
define dso_local noundef i32 @main() #0 {
  %result = alloca i32, align 4
  %t = alloca %struct.Test, align 8
  %res = alloca i32, align 4
  store i32 0, ptr %result, align 4
  store %struct.Test { i32 12 }, ptr %t, align 4
  %1 = call noundef ptr @_ZN4Test8getFieldEv(ptr noundef nonnull align 4 dereferenceable(4) %t)
  %2 = load i32, ptr %1, align 4
  store i32 %2, ptr %res, align 4
  %3 = load i32, ptr %res, align 4
  %4 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i32 noundef %3)
  %5 = load i32, ptr %result, align 4
  ret i32 %5
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr noundef readonly captures(none), ...) local_unnamed_addr #1

attributes #0 = { mustprogress noinline norecurse nounwind optnone uwtable }
attributes #1 = { nofree nounwind }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 2}
!4 = !{!"spice version dev (https://github.com/spicelang/spice)"}
