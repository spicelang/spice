; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-none"

%union.Value = type { i32, [0 x i32], [4 x i8] }
%struct.Error = type { i32, ptr }

@union.tag.msg.0 = private unnamed_addr constant [53 x i8] c"Active field mismatch on union field access 'number'\00", align 4
@assert.msg.0 = private unnamed_addr constant [71 x i8] c"Assertion failed: Condition 'getNumber(value) != 3' evaluated to false\00", align 4
@anon.string.0 = private unnamed_addr constant [6 x i8] c"Boom!\00", align 4

; Function Attrs: noinline nounwind optnone
define internal noundef i32 @_Z9getNumberRK5Value(ptr noundef %0) #0 {
  %value = alloca ptr, align 8
  store ptr %0, ptr %value, align 8
  %2 = load ptr, ptr %value, align 8
  %3 = load i32, ptr %2, align 4
  %4 = icmp eq i32 %3, 1
  br i1 %4, label %union.tag.ok.L12, label %union.tag.panic.L12, !prof !2

union.tag.panic.L12:                              ; preds = %1
  call void @llvm.trap()
  unreachable

union.tag.ok.L12:                                 ; preds = %1
  %number.addr = getelementptr inbounds nuw %union.Value, ptr %2, i32 0, i32 2
  %5 = load i32, ptr %number.addr, align 4
  ret i32 %5
}

; Function Attrs: cold noreturn nounwind memory(inaccessiblemem: write)
declare void @llvm.trap() #1

; Function Attrs: noinline noreturn nounwind optnone
define void @_start() #2 {
  %value = alloca %union.Value, align 8
  %1 = alloca %struct.Error, align 8
  store %union.Value zeroinitializer, ptr %value, align 4
  store i32 2, ptr %value, align 4
  %flag.addr = getelementptr inbounds nuw %union.Value, ptr %value, i32 0, i32 2
  store i1 true, ptr %flag.addr, align 1
  %2 = call noundef i32 @_Z9getNumberRK5Value(ptr noundef %value)
  %3 = icmp ne i32 %2, 3
  br i1 %3, label %assert.exit.L19, label %assert.then.L19, !prof !2

assert.then.L19:                                  ; preds = %0
  call void @llvm.trap()
  unreachable

assert.exit.L19:                                  ; preds = %0
  call void @_ZN5Error4ctorEPKc(ptr noundef nonnull align 8 dereferenceable(16) %1, ptr noundef @anon.string.0)
  %4 = getelementptr inbounds nuw %struct.Error, ptr %1, i32 0, i32 1
  %5 = load ptr, ptr %4, align 8
  call void @llvm.trap()
  unreachable
}

declare void @_ZN5Error4ctorEPKc(ptr, ptr)

attributes #0 = { noinline nounwind optnone }
attributes #1 = { cold noreturn nounwind memory(inaccessiblemem: write) }
attributes #2 = { noinline noreturn nounwind optnone }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 7, !"frame-pointer", i32 0}
!1 = !{!"spice version dev (https://github.com/spicelang/spice)"}
!2 = !{!"branch_weights", i32 1048575, i32 1}
