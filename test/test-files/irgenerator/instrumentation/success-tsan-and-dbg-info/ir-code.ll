; ModuleID = 'source.spice'
source_filename = "source.spice"

%struct.Thread = type { %struct.Lambda, i64 }
%struct.Lambda = type { { ptr, ptr, i64 }, ptr, i64 }

@COUNTER = private global i32 0, !dbg !0
@llvm.used = appending global [1 x ptr] [ptr @tsan.module_ctor], section "llvm.metadata"
@llvm.global_ctors = appending global [1 x { i32, ptr, ptr }] [{ i32, ptr, ptr } { i32 0, ptr @tsan.module_ctor, ptr null }]

; Function Attrs: sanitize_thread
define private void @_Z6workerv() #0 !dbg !15 {
  %1 = call ptr @llvm.returnaddress(i32 0), !dbg !20
  call void @__tsan_func_entry(ptr %1), !dbg !20
  %i = alloca i32, align 4
  store i32 0, ptr %i, align 4, !dbg !21
    #dbg_declare(ptr %i, !23, !DIExpression(), !24)
  br label %for.head.L8, !dbg !24

for.head.L8:                                      ; preds = %for.tail.L8, %0
  %2 = load i32, ptr %i, align 4, !dbg !25
  %3 = icmp slt i32 %2, 1000000, !dbg !25
  br i1 %3, label %for.body.L8, label %for.exit.L8, !dbg !25

for.body.L8:                                      ; preds = %for.head.L8
  %4 = load i32, ptr @COUNTER, align 4, !dbg !26
  %5 = add nsw i32 %4, 1, !dbg !26
  call void @__tsan_write4(ptr @COUNTER), !dbg !26
  store i32 %5, ptr @COUNTER, align 4, !dbg !26
  br label %for.tail.L8, !dbg !27

for.tail.L8:                                      ; preds = %for.body.L8
  %6 = load i32, ptr %i, align 4, !dbg !28
  %7 = add nsw i32 %6, 1, !dbg !28
  store i32 %7, ptr %i, align 4, !dbg !28
  br label %for.head.L8, !dbg !28

for.exit.L8:                                      ; preds = %for.head.L8
  call void @__tsan_func_exit(), !dbg !29
  ret void, !dbg !29
}

; Function Attrs: mustprogress noinline norecurse nounwind optnone sanitize_thread uwtable
define dso_local noundef i32 @main() #1 !dbg !30 {
  %1 = call ptr @llvm.returnaddress(i32 0), !dbg !33
  call void @__tsan_func_entry(ptr %1), !dbg !33
  %result = alloca i32, align 4
  %thread1 = alloca %struct.Thread, align 8
  %fat.ptr = alloca { ptr, ptr, i64 }, align 8
  %thread2 = alloca %struct.Thread, align 8
  %fat.ptr1 = alloca { ptr, ptr, i64 }, align 8
    #dbg_declare(ptr %result, !34, !DIExpression(), !35)
  store i32 0, ptr %result, align 4, !dbg !35
  store ptr @_Z6workerv.fatthunk, ptr %fat.ptr, align 8, !dbg !36
  %2 = getelementptr inbounds nuw { ptr, ptr, i64 }, ptr %fat.ptr, i32 0, i32 1, !dbg !36
  store ptr null, ptr %2, align 8, !dbg !36
  %3 = getelementptr inbounds nuw { ptr, ptr, i64 }, ptr %fat.ptr, i32 0, i32 2, !dbg !36
  store i64 0, ptr %3, align 8, !dbg !36
  %4 = load { ptr, ptr, i64 }, ptr %fat.ptr, align 8, !dbg !36
  call void @_ZN6Thread4ctorEPFvE(ptr noundef nonnull align 8 dereferenceable(48) %thread1, { ptr, ptr, i64 } noundef %4), !dbg !36
    #dbg_declare(ptr %thread1, !37, !DIExpression(), !57)
  store ptr @_Z6workerv.fatthunk, ptr %fat.ptr1, align 8, !dbg !58
  %5 = getelementptr inbounds nuw { ptr, ptr, i64 }, ptr %fat.ptr1, i32 0, i32 1, !dbg !58
  store ptr null, ptr %5, align 8, !dbg !58
  %6 = getelementptr inbounds nuw { ptr, ptr, i64 }, ptr %fat.ptr1, i32 0, i32 2, !dbg !58
  store i64 0, ptr %6, align 8, !dbg !58
  %7 = load { ptr, ptr, i64 }, ptr %fat.ptr1, align 8, !dbg !58
  call void @_ZN6Thread4ctorEPFvE(ptr noundef nonnull align 8 dereferenceable(48) %thread2, { ptr, ptr, i64 } noundef %7), !dbg !58
    #dbg_declare(ptr %thread2, !59, !DIExpression(), !60)
  call void @_ZN6Thread3runEv(ptr noundef nonnull align 8 dereferenceable(48) %thread1), !dbg !61
  call void @_ZN6Thread3runEv(ptr noundef nonnull align 8 dereferenceable(48) %thread2), !dbg !62
  call void @_ZN6Thread4joinEv(ptr noundef nonnull align 8 dereferenceable(48) %thread1), !dbg !63
  call void @_ZN6Thread4joinEv(ptr noundef nonnull align 8 dereferenceable(48) %thread2), !dbg !64
  call void @_ZN6Thread4dtorEv(ptr noundef nonnull align 8 dereferenceable(48) %thread2), !dbg !65
  call void @_ZN6Thread4dtorEv(ptr noundef nonnull align 8 dereferenceable(48) %thread1), !dbg !65
  %8 = load i32, ptr %result, align 4, !dbg !65
  call void @__tsan_func_exit(), !dbg !65
  ret i32 %8, !dbg !65
}

define private void @_Z6workerv.fatthunk(ptr %0) personality ptr @__gcc_personality_v0 {
entry:
  %1 = call ptr @llvm.returnaddress(i32 0), !dbg !36
  call void @__tsan_func_entry(ptr %1), !dbg !36
  invoke void @_Z6workerv()
          to label %.noexc unwind label %tsan_cleanup, !dbg !36

.noexc:                                           ; preds = %entry
  call void @__tsan_func_exit(), !dbg !36
  ret void, !dbg !36

tsan_cleanup:                                     ; preds = %entry
  %cleanup.lpad = landingpad { ptr, i32 }
          cleanup
  call void @__tsan_func_exit()
  resume { ptr, i32 } %cleanup.lpad
}

declare void @_ZN6Thread4ctorEPFvE(ptr, { ptr, ptr, i64 })

declare void @_ZN6Thread3runEv(ptr)

declare void @_ZN6Thread4joinEv(ptr)

declare void @_ZN6Thread4dtorEv(ptr noundef nonnull align 8 dereferenceable(48))

declare void @__tsan_init()

; Function Attrs: nounwind uwtable
define internal void @tsan.module_ctor() #2 {
  call void @__tsan_init()
  ret void
}

; Function Attrs: nounwind
declare void @__tsan_func_entry(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_func_exit() #3

; Function Attrs: nounwind
declare void @__tsan_ignore_thread_begin() #3

; Function Attrs: nounwind
declare void @__tsan_ignore_thread_end() #3

; Function Attrs: nounwind
declare void @__tsan_read1(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_write1(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_unaligned_read1(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_unaligned_write1(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_volatile_read1(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_volatile_write1(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_unaligned_volatile_read1(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_unaligned_volatile_write1(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_read_write1(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_unaligned_read_write1(ptr) #3

; Function Attrs: nounwind
declare i8 @__tsan_atomic8_load(ptr, i32) #3

; Function Attrs: nounwind
declare void @__tsan_atomic8_store(ptr, i8, i32) #3

; Function Attrs: nounwind
declare i8 @__tsan_atomic8_exchange(ptr, i8, i32) #3

; Function Attrs: nounwind
declare i8 @__tsan_atomic8_fetch_add(ptr, i8, i32) #3

; Function Attrs: nounwind
declare i8 @__tsan_atomic8_fetch_sub(ptr, i8, i32) #3

; Function Attrs: nounwind
declare i8 @__tsan_atomic8_fetch_and(ptr, i8, i32) #3

; Function Attrs: nounwind
declare i8 @__tsan_atomic8_fetch_nand(ptr, i8, i32) #3

; Function Attrs: nounwind
declare i8 @__tsan_atomic8_fetch_or(ptr, i8, i32) #3

; Function Attrs: nounwind
declare i8 @__tsan_atomic8_fetch_xor(ptr, i8, i32) #3

; Function Attrs: nounwind
declare i8 @__tsan_atomic8_compare_exchange_val(ptr, i8, i8, i32, i32) #3

; Function Attrs: nounwind
declare void @__tsan_read2(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_write2(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_unaligned_read2(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_unaligned_write2(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_volatile_read2(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_volatile_write2(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_unaligned_volatile_read2(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_unaligned_volatile_write2(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_read_write2(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_unaligned_read_write2(ptr) #3

; Function Attrs: nounwind
declare i16 @__tsan_atomic16_load(ptr, i32) #3

; Function Attrs: nounwind
declare void @__tsan_atomic16_store(ptr, i16, i32) #3

; Function Attrs: nounwind
declare i16 @__tsan_atomic16_exchange(ptr, i16, i32) #3

; Function Attrs: nounwind
declare i16 @__tsan_atomic16_fetch_add(ptr, i16, i32) #3

; Function Attrs: nounwind
declare i16 @__tsan_atomic16_fetch_sub(ptr, i16, i32) #3

; Function Attrs: nounwind
declare i16 @__tsan_atomic16_fetch_and(ptr, i16, i32) #3

; Function Attrs: nounwind
declare i16 @__tsan_atomic16_fetch_nand(ptr, i16, i32) #3

; Function Attrs: nounwind
declare i16 @__tsan_atomic16_fetch_or(ptr, i16, i32) #3

; Function Attrs: nounwind
declare i16 @__tsan_atomic16_fetch_xor(ptr, i16, i32) #3

; Function Attrs: nounwind
declare i16 @__tsan_atomic16_compare_exchange_val(ptr, i16, i16, i32, i32) #3

; Function Attrs: nounwind
declare void @__tsan_read4(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_write4(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_unaligned_read4(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_unaligned_write4(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_volatile_read4(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_volatile_write4(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_unaligned_volatile_read4(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_unaligned_volatile_write4(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_read_write4(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_unaligned_read_write4(ptr) #3

; Function Attrs: nounwind
declare i32 @__tsan_atomic32_load(ptr, i32) #3

; Function Attrs: nounwind
declare void @__tsan_atomic32_store(ptr, i32, i32) #3

; Function Attrs: nounwind
declare i32 @__tsan_atomic32_exchange(ptr, i32, i32) #3

; Function Attrs: nounwind
declare i32 @__tsan_atomic32_fetch_add(ptr, i32, i32) #3

; Function Attrs: nounwind
declare i32 @__tsan_atomic32_fetch_sub(ptr, i32, i32) #3

; Function Attrs: nounwind
declare i32 @__tsan_atomic32_fetch_and(ptr, i32, i32) #3

; Function Attrs: nounwind
declare i32 @__tsan_atomic32_fetch_nand(ptr, i32, i32) #3

; Function Attrs: nounwind
declare i32 @__tsan_atomic32_fetch_or(ptr, i32, i32) #3

; Function Attrs: nounwind
declare i32 @__tsan_atomic32_fetch_xor(ptr, i32, i32) #3

; Function Attrs: nounwind
declare i32 @__tsan_atomic32_compare_exchange_val(ptr, i32, i32, i32, i32) #3

; Function Attrs: nounwind
declare void @__tsan_read8(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_write8(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_unaligned_read8(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_unaligned_write8(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_volatile_read8(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_volatile_write8(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_unaligned_volatile_read8(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_unaligned_volatile_write8(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_read_write8(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_unaligned_read_write8(ptr) #3

; Function Attrs: nounwind
declare i64 @__tsan_atomic64_load(ptr, i32) #3

; Function Attrs: nounwind
declare void @__tsan_atomic64_store(ptr, i64, i32) #3

; Function Attrs: nounwind
declare i64 @__tsan_atomic64_exchange(ptr, i64, i32) #3

; Function Attrs: nounwind
declare i64 @__tsan_atomic64_fetch_add(ptr, i64, i32) #3

; Function Attrs: nounwind
declare i64 @__tsan_atomic64_fetch_sub(ptr, i64, i32) #3

; Function Attrs: nounwind
declare i64 @__tsan_atomic64_fetch_and(ptr, i64, i32) #3

; Function Attrs: nounwind
declare i64 @__tsan_atomic64_fetch_nand(ptr, i64, i32) #3

; Function Attrs: nounwind
declare i64 @__tsan_atomic64_fetch_or(ptr, i64, i32) #3

; Function Attrs: nounwind
declare i64 @__tsan_atomic64_fetch_xor(ptr, i64, i32) #3

; Function Attrs: nounwind
declare i64 @__tsan_atomic64_compare_exchange_val(ptr, i64, i64, i32, i32) #3

; Function Attrs: nounwind
declare void @__tsan_read16(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_write16(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_unaligned_read16(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_unaligned_write16(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_volatile_read16(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_volatile_write16(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_unaligned_volatile_read16(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_unaligned_volatile_write16(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_read_write16(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_unaligned_read_write16(ptr) #3

; Function Attrs: nounwind
declare i128 @__tsan_atomic128_load(ptr, i32) #3

; Function Attrs: nounwind
declare void @__tsan_atomic128_store(ptr, i128, i32) #3

; Function Attrs: nounwind
declare i128 @__tsan_atomic128_exchange(ptr, i128, i32) #3

; Function Attrs: nounwind
declare i128 @__tsan_atomic128_fetch_add(ptr, i128, i32) #3

; Function Attrs: nounwind
declare i128 @__tsan_atomic128_fetch_sub(ptr, i128, i32) #3

; Function Attrs: nounwind
declare i128 @__tsan_atomic128_fetch_and(ptr, i128, i32) #3

; Function Attrs: nounwind
declare i128 @__tsan_atomic128_fetch_nand(ptr, i128, i32) #3

; Function Attrs: nounwind
declare i128 @__tsan_atomic128_fetch_or(ptr, i128, i32) #3

; Function Attrs: nounwind
declare i128 @__tsan_atomic128_fetch_xor(ptr, i128, i32) #3

; Function Attrs: nounwind
declare i128 @__tsan_atomic128_compare_exchange_val(ptr, i128, i128, i32, i32) #3

; Function Attrs: nounwind
declare void @__tsan_vptr_update(ptr, ptr) #3

; Function Attrs: nounwind
declare void @__tsan_vptr_read(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_atomic_thread_fence(i32) #3

; Function Attrs: nounwind
declare void @__tsan_atomic_signal_fence(i32) #3

; Function Attrs: nounwind
declare ptr @__tsan_memmove(ptr, ptr, i64) #3

; Function Attrs: nounwind
declare ptr @__tsan_memcpy(ptr, ptr, i64) #3

; Function Attrs: nounwind
declare ptr @__tsan_memset(ptr, i32, i64) #3

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(none)
declare ptr @llvm.returnaddress(i32 immarg) #4

declare i32 @__gcc_personality_v0(...)

attributes #0 = { sanitize_thread }
attributes #1 = { mustprogress noinline norecurse nounwind optnone sanitize_thread uwtable }
attributes #2 = { nounwind uwtable "frame-pointer"="all" }
attributes #3 = { nounwind }
attributes #4 = { nocallback nofree nosync nounwind willreturn memory(none) }

!llvm.module.flags = !{!7, !8, !9, !10, !11, !12, !13}
!llvm.ident = !{!14}
!llvm.dbg.cu = !{!2}

!0 = !DIGlobalVariableExpression(var: !1, expr: !DIExpression())
!1 = distinct !DIGlobalVariable(name: "COUNTER", linkageName: "COUNTER", scope: !2, file: !5, line: 5, type: !6, isLocal: false, isDefinition: true)
!2 = distinct !DICompileUnit(language: DW_LANG_C_plus_plus_14, file: !3, producer: "spice version dev (https://github.com/spicelang/spice)", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, globals: !4, splitDebugInlining: false, nameTableKind: None)
!3 = !DIFile(filename: "/home/marc/Documents/Dev/spice/cmake-build-debug/test/./test-files/irgenerator/instrumentation/success-tsan-and-dbg-info/source.spice", directory: "./test-files/irgenerator/instrumentation/success-tsan-and-dbg-info")
!4 = !{!0}
!5 = !DIFile(filename: "source.spice", directory: "./test-files/irgenerator/instrumentation/success-tsan-and-dbg-info")
!6 = !DIBasicType(name: "int", size: 32, encoding: DW_ATE_signed)
!7 = !{i32 8, !"PIC Level", i32 2}
!8 = !{i32 7, !"PIE Level", i32 2}
!9 = !{i32 7, !"uwtable", i32 2}
!10 = !{i32 7, !"frame-pointer", i32 2}
!11 = !{i32 7, !"Dwarf Version", i32 5}
!12 = !{i32 2, !"Debug Info Version", i32 3}
!13 = !{i32 4, !"nosanitize_thread", i32 1}
!14 = !{!"spice version dev (https://github.com/spicelang/spice)"}
!15 = distinct !DISubprogram(name: "worker", linkageName: "_Z6workerv", scope: !5, file: !5, line: 7, type: !16, scopeLine: 7, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !19)
!16 = !DISubroutineType(types: !17)
!17 = !{!18}
!18 = !DIBasicType(name: "void", encoding: DW_ATE_unsigned)
!19 = !{}
!20 = !DILocation(line: 0, scope: !15)
!21 = !DILocation(line: 8, column: 17, scope: !22)
!22 = distinct !DILexicalBlock(scope: !15, file: !5, line: 8, column: 5)
!23 = !DILocalVariable(name: "i", scope: !22, file: !5, line: 8, type: !6)
!24 = !DILocation(line: 8, column: 9, scope: !22)
!25 = !DILocation(line: 8, column: 24, scope: !22)
!26 = !DILocation(line: 9, column: 9, scope: !22)
!27 = !DILocation(line: 10, column: 5, scope: !22)
!28 = !DILocation(line: 8, column: 33, scope: !22)
!29 = !DILocation(line: 11, column: 1, scope: !15)
!30 = distinct !DISubprogram(name: "main", linkageName: "_Z4mainv", scope: !5, file: !5, line: 13, type: !31, scopeLine: 13, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !19)
!31 = !DISubroutineType(types: !32)
!32 = !{!6}
!33 = !DILocation(line: 0, scope: !30)
!34 = !DILocalVariable(name: "result", scope: !30, file: !5, line: 13, type: !6)
!35 = !DILocation(line: 13, column: 1, scope: !30)
!36 = !DILocation(line: 14, column: 29, scope: !30)
!37 = !DILocalVariable(name: "thread1", scope: !30, file: !5, line: 14, type: !38)
!38 = !DICompositeType(tag: DW_TAG_structure_type, name: "Thread", scope: !5, file: !5, line: 23, size: 384, align: 8, flags: DIFlagTypePassByReference | DIFlagNonTrivial, elements: !39, identifier: "struct.Thread")
!39 = !{!40, !55}
!40 = !DIDerivedType(tag: DW_TAG_member, name: "threadRoutine", scope: !38, file: !5, line: 24, baseType: !41, size: 320, align: 8)
!41 = !DICompositeType(tag: DW_TAG_structure_type, name: "Lambda", scope: !5, file: !5, line: 31, size: 320, align: 8, flags: DIFlagTypePassByReference | DIFlagNonTrivial, elements: !42, identifier: "struct.Lambda")
!42 = !{!43, !51, !54}
!43 = !DIDerivedType(tag: DW_TAG_member, name: "native", scope: !41, file: !5, line: 32, baseType: !44, size: 192, align: 8)
!44 = !DICompositeType(tag: DW_TAG_structure_type, name: "_lambda", scope: !5, file: !5, size: 192, align: 8, flags: DIFlagTypePassByValue | DIFlagNonTrivial, elements: !45, identifier: "_lambda")
!45 = !{!46, !48, !49}
!46 = !DIDerivedType(tag: DW_TAG_member, name: "fct", scope: !44, file: !5, baseType: !47, size: 64, align: 8)
!47 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !18, size: 64, align: 8)
!48 = !DIDerivedType(tag: DW_TAG_member, name: "captures", scope: !44, file: !5, baseType: !47, size: 64, align: 8, offset: 64)
!49 = !DIDerivedType(tag: DW_TAG_member, name: "captureSize", scope: !44, file: !5, baseType: !50, size: 64, align: 8, offset: 128)
!50 = !DIBasicType(name: "unsigned long", size: 64, encoding: DW_ATE_unsigned)
!51 = !DIDerivedType(tag: DW_TAG_member, name: "ownedCaptures", scope: !41, file: !5, line: 33, baseType: !52, size: 64, offset: 192)
!52 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !53, size: 64)
!53 = !DIBasicType(name: "byte", size: 8, encoding: DW_ATE_unsigned)
!54 = !DIDerivedType(tag: DW_TAG_member, name: "captureSize", scope: !41, file: !5, line: 34, baseType: !50, size: 64, offset: 256)
!55 = !DIDerivedType(tag: DW_TAG_member, name: "threadId", scope: !38, file: !5, line: 25, baseType: !56, size: 64, offset: 320)
!56 = !DIBasicType(name: "long", size: 64, encoding: DW_ATE_signed)
!57 = !DILocation(line: 14, column: 5, scope: !30)
!58 = !DILocation(line: 15, column: 29, scope: !30)
!59 = !DILocalVariable(name: "thread2", scope: !30, file: !5, line: 15, type: !38)
!60 = !DILocation(line: 15, column: 5, scope: !30)
!61 = !DILocation(line: 16, column: 5, scope: !30)
!62 = !DILocation(line: 17, column: 5, scope: !30)
!63 = !DILocation(line: 18, column: 5, scope: !30)
!64 = !DILocation(line: 19, column: 5, scope: !30)
!65 = !DILocation(line: 20, column: 1, scope: !30)
